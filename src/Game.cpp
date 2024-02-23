#include "Game.hpp"
#include "TiledMapManager.hpp"
#include "Collision.hpp"
#include "ECS/Components.hpp"

SDL_Renderer *Game::renderer = nullptr;

bool Game::isRunning = false;
bool Game::isDebug = false;
Camera2D Game::camera;

Manager manager;

auto &player(manager.addEntity());

const std::string levelMap = "assets/tiled/testmap.tmx";

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
    mapManager.loadMap(levelMap, 2, true);

    player.addGroup(groupPlayers);
    player.addComponent<TransformComponent>(static_cast<float>(windowWidth) / 2, static_cast<float>(windowHeight) / 2, 32.0f, 32.0f, 2);
    player.addComponent<SpriteComponent>("assets/sprites/player_anim.png", true, true);
    player.addComponent<ColliderComponent>("player");
    player.addComponent<AnchorComponent>("playerAnchor", AnchorComponent::AnchorBottom);
    player.addComponent<KeyboardController>();
}

auto& mapTiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::groupEnemies));

void Game::update() const
{
    Vector2D playerPos = player.GetComponent<TransformComponent>().position;
    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        if (Collision::AABB(c->GetComponent<ColliderComponent>(), player.GetComponent<AnchorComponent>()))
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
