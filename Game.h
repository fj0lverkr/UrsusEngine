#ifndef Game_hpp
#define Game_hpp
#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Game
{
private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int cnt = 0;

public:
    Game(/* args */);
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running();
};

#endif /* Game_hpp */