#include "Game.h"
#include "GameObject.h"
#include "TiledMap.h"

GameObject *player;
TiledMap *map;
SDL_Renderer *Game::renderer = nullptr;

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

    player = new GameObject("assets/player_sprite.png", 0, 0, 0, 0, 32, 32);
    map = new TiledMap();
}

void Game::handleEvents()
{
    SDL_Event event;
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
    player->Update();
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // Add content to render, content is rendered from back to front
    map->DrawMap();
    player->Render();
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
