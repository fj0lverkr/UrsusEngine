#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{
    const int TARGETFPS = 60;
    const int FRAMEDELAY = 1000 / TARGETFPS;

    Uint64 frameStart;
    int frameTime;

    game = new Game();
    game->init("Ursus Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
    while (game->running())
    {
        frameStart = SDL_GetTicks64();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks64() - frameStart;

        if (FRAMEDELAY > frameTime)
        {
            SDL_Delay(FRAMEDELAY - frameTime);
        }
    }
    game->clean();
    return 0;
}
