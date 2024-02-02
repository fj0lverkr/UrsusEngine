#pragma once

#include "Game.h"
#include "TextureManager.h"

class GameObject
{
private:
    int xpos;
    int ypos;
    SDL_Texture *objTexture;
    SDL_Rect srcRect, destRect;

public:
    GameObject(const char *textureSheet, int x, int y, int srcX, int srcY, int srcW, int srcH);
    ~GameObject();

    void Update();
    void Render();
};