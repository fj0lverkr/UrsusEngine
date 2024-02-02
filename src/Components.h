#pragma once

#include "ECS.h"

class PositionComponent : public Component
{
private:
    int posx, posy;

public:
    int x() { return posx; }
    int y() { return posy; }

    void init() override
    {
        posx = posy = 0;
    }

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