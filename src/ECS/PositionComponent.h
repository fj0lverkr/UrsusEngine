#pragma once

#include "Components.h"

class PositionComponent : public Component
{
private:
    int posx, posy;

public:
    PositionComponent()
    {
        posx = posy = 0;
    }

    PositionComponent(int x, int y)
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