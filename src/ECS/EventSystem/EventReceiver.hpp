#pragma once
#include <SDL2/sdl.h>

class EventReceiver
{
public:
    virtual bool handleEvent(const SDL_Event* e)
    {
        return false;
    }
};