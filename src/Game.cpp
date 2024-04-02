#include <sstream>
#include "Game.hpp"
#include "TiledMapManager.hpp"
#include "Collision.hpp"
#include "ECS/Components.hpp"
#include "ECS/Animation.hpp"

SDL_Renderer *Game::renderer = nullptr;

bool Game::isRunning = false;
bool Game::isDebug = false;
int Game::scaleFactor = 0;
Camera2D Game::camera;
SDL_Event Game::event;

Manager manager;
AssetManager* Game::assets = new AssetManager(&manager);
KeyboardController Game::keyboardController;

UILabel* Game::debugLabel = nullptr;
std::vector<Entity*> ysortables;

auto& player(manager.addEntity());
auto& debugHud(manager.addEntity());

TiledMapManager mapManager;

Game::Game()
{
    window = {};
    windowHeight = windowWidth = 0;
}

Game::~Game()
{
}

struct
{
    bool operator()(Entity *e1, Entity *e2) const
    {
        // TODO check to see if we can improve this.
        float e1Min = e1->GetYSortValue("MIN");
        float e2Min = e2->GetYSortValue("MIN");
        return (e1Min < e2Min);
    }
}
ysortEntities;

void Game::init(const char *title, int xpos, int ypos, int width, int height, int scaleFactor, bool fullscreen, SDL_Color rendererColor, bool debug)
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
            this->scaleFactor = scaleFactor;
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer)
            {
                SDL_SetRenderDrawColor(renderer, rendererColor.r, rendererColor.g, rendererColor.b, rendererColor.a);
                isRunning = true;
                Game::camera = {0.0f, 0.0f, width, height};
                if (TTF_Init() == -1)
                {
                    std::cout << "Error initialtizing SDL_ttf, font rendering might be disabled." << std::endl;
                }
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
    Game::assets->AddFont("Swansea_16", "assets/fonts/Swansea.ttf", 16);

    mapManager.LoadMap("testmap", scaleFactor, debug);

    player.AddGroup(groupPlayers);
    debugHud.AddGroup(groupUI);

    // setup player sprite animation
    std::map<const char*, Animation> playerAnimations;
    Animation idle = Animation(0, 9, 100);
    Animation walkSide = Animation(1, 7, 100);
    Animation walkFront = Animation(2, 7, 100);
    playerAnimations.emplace("Idle", idle);
    playerAnimations.emplace("WalkSide", walkSide);
    playerAnimations.emplace("WalkFront", walkFront);

    player.addComponent<TransformComponent>(static_cast<float>(windowWidth) / 2, static_cast<float>(windowHeight) / 2, 32.0f, 32.0f, scaleFactor);
    player.addComponent<SpriteComponent>("PlayerSprite", playerAnimations, "Idle");
    player.addComponent<ColliderComponent>("player", ColliderType::AABB);
    player.addComponent<AnchorComponent>("playerAnchor", AnchorComponent::AnchorBottom, 0.0f, 2.0f, isDebug);
    player.addComponent<KeyboardController>();
    player.addComponent<YsortingComponent>();

    keyboardController = player.GetComponent<KeyboardController>();

    debugHud.addComponent<TransformComponent>(10.0f, 10.0f);
    debugHud.addComponent<UILabel>("init", "Swansea_16");

    debugLabel = &debugHud.GetComponent<UILabel>();
}

auto& mapTiles(manager.getGroup(Game::groupMapTiles));
auto& mapObjects(manager.getGroup(Game::groupMapObjects));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& uiElements(manager.getGroup(Game::groupUI));

void Game::update(int fps) const
{
    std::stringstream ss;

    if (isDebug)
    {
        ss << "Player: " << player.GetComponent<AnchorComponent>().collider.x << " x, " << player.GetComponent<AnchorComponent>().collider.y << " y " << " | FPS: " << fps;
        debugLabel->setLabelText(ss.str());
    }

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

    // IN the future we will handle collision with mapobjects differently (e.g. some objects might be pushable)
    for (auto& o : mapObjects)
    {
        if (Collision::AABB(o->GetComponent<ColliderComponent>(), player.GetComponent<AnchorComponent>()))
        {
            player.GetComponent<TransformComponent>().position = playerPos;
        }
        if (Collision::PointCollision(o->GetComponent<ColliderComponent>(), player.GetComponent<AnchorComponent>()))
        {
            player.GetComponent<TransformComponent>().position = playerPos;
        }
    }

    camera.follow(player.GetComponent<TransformComponent>().position, windowWidth, windowHeight);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    ysortables.clear();

    for (auto &t : mapTiles)
    {
        t->draw();
    }

    if (isDebug)
    {
        for (auto& c : colliders)
        {
            c->draw();
        }
    }

    for (auto &o : mapObjects)
    {
        ysortables.emplace_back(o);
    }
    for (auto &p : players)
    {
        ysortables.emplace_back(p);
    }
    for (auto &e : enemies)
    {
        ysortables.emplace_back(e);
    }

    std::sort(ysortables.begin(), ysortables.end(), ysortEntities);
    for (auto &d : ysortables)
    {
        d->draw();
    }

    for (auto &ui : uiElements)
    {
        ui->draw();
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
