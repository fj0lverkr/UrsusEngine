#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Camera2D.hpp"

class ColliderComponent;

class Game
{
private:
    SDL_Window *window;
    int cnt = 0;
    int windowWidth, windowHeight;

public:
    Game(/* args */);
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Color rendererColor);
    void update();
    void render();
    void clean();

    static void AddTile(int srcX, int srcY, int x, int y, const char* tilesetPath, int tileSize, int scaleFactor);
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent *> colliders;
    static bool isRunning;
    static Camera2D camera;
};