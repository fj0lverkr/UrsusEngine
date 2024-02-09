#include "Game.h"
#include "TiledMap.h"
#include "Collision.h"
#include "ECS/Components.h"
#include <string>

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent *> Game::colliders;

bool Game::isRunning = false;

Manager manager;
auto &player(manager.addEntity());

const char *mapTileset = "assets/map/tiles.png";

// Labels for grouping Entities, we can have up to 32 Groups per Entity
enum groupLabels : std::size_t
{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
};

auto &mapTiles(manager.getGroup(groupMap));
auto &players(manager.getGroup(groupPlayers));
auto &colliderEntities(manager.getGroup(groupColliders));
auto &enemies(manager.getGroup(groupEnemies));

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Color rendererColor)
{
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
            }
        }
    }
    else
    {
        isRunning = false;
    }

    TiledMap::LoadMap("assets/map/testmap.map", 30, 20, 2);

    player.addComponent<TransformComponent>(windowWidth / 2, windowHeight / 2, 32, 32, 2);
    player.addComponent<SpriteComponent>("assets/sprites/player_anim.png", true);
    player.addComponent<ColliderComponent>("player");
    player.addComponent<KeyboardController>();
    player.addGroup(groupPlayers);
}

void Game::update()
{
    manager.refresh();
    manager.update();

    Vector2D playerVelocity = player.GetComponent<TransformComponent>().velocity;
    int playerSpeed = player.GetComponent<TransformComponent>().speed;

    for (auto tile : mapTiles)
    {
        tile->GetComponent<TileComponent>().destRect.x += -(playerVelocity.x * playerSpeed);
        tile->GetComponent<TileComponent>().destRect.y += -(playerVelocity.y * playerSpeed);
    }
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
    for (auto &c : colliderEntities)
    {
        c->draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::AddTile(int srcX, int srcY, int x, int y)
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, x, y, mapTileset, 2);
    tile.addGroup(groupMap);
}
