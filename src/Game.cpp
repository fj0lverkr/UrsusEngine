#include "Game.hpp"
#include "TiledMapManager.hpp"
#include "Collision.hpp"
#include "ECS/Components.hpp"
#include <string>

SDL_Renderer *Game::renderer = nullptr;

std::vector<ColliderComponent *> Game::colliders;

bool Game::isRunning = false;
Camera2D Game::camera;

Manager manager;

auto &player(manager.addEntity());

const std::string levelMap = "assets/tiled/testmap.tmx";

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
                Game::camera = {0, 0, width, height};
            }
        }
    }
    else
    {
        isRunning = false;
    }

    TiledMapManager::loadMap(levelMap, 2);

    player.addComponent<TransformComponent>(windowWidth / 2, windowHeight / 2, 32, 32, 2);
    player.addComponent<SpriteComponent>("assets/sprites/player_anim.png", true, true);
    player.addComponent<ColliderComponent>("player");
    player.addComponent<KeyboardController>();
    player.addGroup(groupPlayers);
}

void Game::update() const
{
    manager.refresh();
    manager.update();

    for (auto& c : colliderEntities) {
        if (Collision::AABB(c->GetComponent<ColliderComponent>(), player.GetComponent<ColliderComponent>()))
        {
            player.GetComponent<TransformComponent>().velocity.Zero();
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

void Game::AddTile(int srcX, int srcY, int x, int y, const char *tilesetPath, int tileSize, int scaleFactor)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, x, y, tilesetPath, tileSize, scaleFactor);
    tile.addGroup(groupMap);
}

void Game::AddTile(int srcX, int srcY, int x, int y, const char* tilesetPath, int tileSize, int scaleFactor, std::vector<SDL_Rect> colliders)
{
    Game::AddTile(srcX, srcY, x, y, tilesetPath, tileSize, scaleFactor);
    for (SDL_Rect collider : colliders)
    {
        auto& box(manager.addEntity());
        int boxX = x + collider.x;
        int boxY = y + collider.y;
        box.addComponent<TransformComponent>(boxX, boxY, collider.w, collider.y, scaleFactor);
        box.addComponent<SpriteComponent>("assets/placeholder.png", false);
        box.addComponent<ColliderComponent>("TileCollision");
        box.addGroup(groupColliders);
    }
}
