#pragma once

#include "Components.h"

class TransformComponent : public Component
{
private:
    int posx, posy;

public:
    TransformComponent()
    {
        posx = posy = 0;
    }

    TransformComponent(int x, int y)
    {
        posx = x;
        posy = y;
    }

    int x() { return posx; }
    int y() { return posy; }

    void update()
    {
        posx++;
        posy++;
    }

    void setPos(int x, int y)
    {
        posx = x;
        posy = y;
    }
};