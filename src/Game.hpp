#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Camera2D.hpp"
#include "AssetManager.hpp"

class ColliderComponent;
class AssetManager;
class KeyboardController;
class UILabel;
class Entity;

class Game
{
private:
    static bool isDebug;
    static KeyboardController keyboardController;
    static UILabel* debugLabel;
    static int scaleFactor;
    SDL_Window* window;
    int cnt = 0;
    int windowWidth, windowHeight;
    static bool ysortEntities(Entity *e1, Entity *e2);

public:
    Game();
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height, int scaleFactor, bool fullscreen, SDL_Color rendererColor, bool debug);
    void update(int fps) const;
    void render();
    void clean();

    static SDL_Renderer* renderer;
    static AssetManager* assets;
    static SDL_Event event;
    static bool isRunning;
    static Camera2D camera;

    // Labels for grouping Entities, we can have up to 32 Groups per Entity
    enum groupLabels : std::size_t
    {
        groupMapTiles,
        groupMapObjects,
        groupPlayers,
        groupEnemies,
        groupColliders,
        groupUI
    };
};