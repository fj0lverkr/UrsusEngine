#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class ColliderComponent;

class Game
{
private:
    bool isRunning;
    SDL_Window *window;
    int cnt = 0;

public:
    Game(/* args */);
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Color rendererColor);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running();
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent *> colliders;
};