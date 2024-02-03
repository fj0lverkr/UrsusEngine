#include "Game.h"
#include "TiledMap.h"
#include "Collision.h"

#include "ECS/Components.h"

TiledMap *map;
SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent *> Game::colliders;

Manager manager;
auto &player(manager.addEntity());
auto &wall(manager.addEntity());

auto &tile0(manager.addEntity());
auto &tile1(manager.addEntity());
auto &tile2(manager.addEntity());

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

    map = new TiledMap();

    player.addComponent<TransformComponent>(10, 10, 32, 32, 2);
    player.addComponent<SpriteComponent>("assets/player_sprite.png");
    player.addComponent<ColliderComponent>("player");
    player.addComponent<KeyboardController>();

    wall.addComponent<TransformComponent>(300, 300, 20, 300, 1);
    wall.addComponent<SpriteComponent>("assets/map/dirt.png");
    wall.addComponent<ColliderComponent>("wall");

    tile0.addComponent<TileComponent>(200, 200, 32, 32, 0);
    tile1.addComponent<TileComponent>(250, 250, 32, 32, 1);
    tile1.addComponent<ColliderComponent>("dirt");
    tile2.addComponent<TileComponent>(150, 150, 32, 32, 2);
    tile2.addComponent<ColliderComponent>("grass");
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();

    for (auto c : colliders)
    {
        for (auto cc : colliders)
        {
            if (Collision::AABB(*c, *cc))
            {
                if (c->tag == "player" || cc->tag == "player")
                {
                    player.GetComponent<TransformComponent>().velocity * -1;
                }
            }
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // Add content to render, content is rendered from back to front
    // map->DrawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

bool Game::running()
{
    return isRunning;
}
