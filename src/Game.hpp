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
    static bool isDebug;
    SDL_Window* window;
    int cnt = 0;
    int windowWidth, windowHeight;

public:
    Game();
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Color rendererColor, bool debug);
    void update() const;
    void render();
    void clean();

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent *> colliders;
    static bool isRunning;
    static Camera2D camera;

    // Labels for grouping Entities, we can have up to 32 Groups per Entity
    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayers,
        groupEnemies,
        groupColliders
    };
};