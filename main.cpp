#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{
    // Framerate settings
    const int TARGETFPS = 60;
    const int FRAMEDELAY = 1000 / TARGETFPS;

    // Initial window Settings
    const char *WINDOWTITLE = "Ursus Engine";
    const int WINDOWW = 800;
    const int WINDOWH = 640;
    const SDL_Color WINDOWBG = {255, 255, 255, 255};

    Uint64 frameStart;
    int frameTime;

    game = new Game();
    game->init(WINDOWTITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWW, WINDOWH, false, WINDOWBG);
    while (game->isRunning)
    {
        frameStart = SDL_GetTicks64();

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
