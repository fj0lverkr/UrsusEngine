#include "Game.hpp"
#include "TiledMapManager.hpp"
#include "Collision.hpp"
#include "ECS/Components.hpp"
#include "ECS//Animation.hpp"

SDL_Renderer *Game::renderer = nullptr;

bool Game::isRunning = false;
bool Game::isDebug = false;
Camera2D Game::camera;
SDL_Event Game::event;

Manager manager;
AssetManager* Game::assets = new AssetManager(&manager);
KeyboardController Game::keyboardController;

auto &player(manager.addEntity());

TiledMapManager mapManager;

Game::Game()
{
    window = {};
    windowHeight = windowWidth = 0;
}

Game::~Game()
{
}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Color rendererColor, bool debug)
{
    isDebug = debug;

    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window)
        {
            windowWidth = width;
            windowHeight = height;
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer)
            {
                SDL_SetRenderDrawColor(renderer, rendererColor.r, rendererColor.g, rendererColor.b, rendererColor.a);
                isRunning = true;
                Game::camera = {0.0f, 0.0f, width, height};
            }
        }
    }
    else
    {
        isRunning = false;
    }
    Game::assets->AddTiledMaps("assets/tiled");
    //TODO move this to the AssetManager and let it load all png files in a given folder.
    Game::assets->AddTexture("PlayerSprite", "assets/sprites/player_anim.png");
    Game::assets->AddTexture("Placeholder", "assets/placeholder.png");

    mapManager.loadMap("testmap", 2, true);

    player.addGroup(groupPlayers);

    // setup player sprite animation
    std::map<const char*, Animation> playerAnimations;
    Animation idle = Animation(0, 9, 100);
    Animation walkSide = Animation(1, 7, 100);
    Animation walkFront = Animation(2, 7, 100);
    playerAnimations.emplace("Idle", idle);
    playerAnimations.emplace("WalkSide", walkSide);
    playerAnimations.emplace("WalkFront", walkFront);

    player.addComponent<TransformComponent>(static_cast<float>(windowWidth) / 2, static_cast<float>(windowHeight) / 2, 32.0f, 32.0f, 2);
    player.addComponent<SpriteComponent>("PlayerSprite", playerAnimations, "Idle");
    player.addComponent<ColliderComponent>("player", ColliderType::AABB);
    player.addComponent<AnchorComponent>("playerAnchor", AnchorComponent::AnchorBottom, 0.0f, 10.0f, isDebug);
    player.addComponent<KeyboardController>();

    keyboardController = player.GetComponent<KeyboardController>();
}

auto& mapTiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::groupEnemies));

void Game::update() const
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            Game::isRunning = false;
        }
        if (keyboardController.HandleEvent(&event))
        {
            continue;
        }
    }

    Vector2D playerPos = player.GetComponent<TransformComponent>().position;
    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        if (Collision::AABB(c->GetComponent<ColliderComponent>(), player.GetComponent<AnchorComponent>()))
        {
            player.GetComponent<TransformComponent>().position = playerPos;
        }
        if (Collision::PointCollision(c->GetComponent<ColliderComponent>(), player.GetComponent<AnchorComponent>()))
        {
            player.GetComponent<TransformComponent>().position = playerPos;
        }
    }

    camera.follow(player.GetComponent<TransformComponent>().position, windowWidth, windowHeight);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &t : mapTiles)
    {
        t->draw();
    }
    for (auto &p : players)
    {
        p->draw();
    }
    for (auto &e : enemies)
    {
        e->draw();
    }
    if (isDebug)
    {
        for (auto& c : colliders)
        {
            c->draw();
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
