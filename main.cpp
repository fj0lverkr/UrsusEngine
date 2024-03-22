#include "Game.hpp"

Game *game = nullptr;

int main(int argc, char *argv[])
{
    // Debug
    const bool DEBUG = true;

    // Framerate settings
    const Uint64 TARGETFPS = 60;
    const Uint64 FRAMEDELAY = 1000 / TARGETFPS;

    // Initial window Settings
    const char *WINDOWTITLE = "Ursus Engine";
    const int WINDOWW = 960;
    const int WINDOWH = 640;
    const int SCALEFACTOR = 2;
    const SDL_Color WINDOWBG = {255, 255, 255, 255};

    Uint64 frameStart;
    Uint64 frameTime;
    Uint64 performanceStart;
    Uint64 performanceEnd;
    float elapsed = 0.0f;
    int fps = 0;

    game = new Game();
    game->init(WINDOWTITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWW, WINDOWH, SCALEFACTOR, false, WINDOWBG, DEBUG);
    while (game->isRunning)
    {
        frameStart = SDL_GetTicks64();
        performanceStart = SDL_GetPerformanceCounter();

        game->update(fps);
        game->render();

        frameTime = SDL_GetTicks64() - frameStart;


        if (FRAMEDELAY > frameTime)
        {
            SDL_Delay(static_cast<Uint32>(FRAMEDELAY - frameTime));
        }

        performanceEnd = SDL_GetPerformanceCounter();
        elapsed = (performanceEnd - performanceStart) / (float)SDL_GetPerformanceFrequency();
        fps = static_cast<int>(1.0f / elapsed);
    }
    game->clean();
    return 0;
}
