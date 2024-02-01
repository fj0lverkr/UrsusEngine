#include "GameObject.h"

GameObject::GameObject(const char *textureSheet, int x, int y, int srcX, int srcY, int srcW, int srcH)
{
    objTexture = TextureManager::LoadTexture(textureSheet);

    xpos = x;
    ypos = y;

    srcRect.h = srcH;
    srcRect.w = srcW;
    srcRect.x = srcX;
    srcRect.y = srcY;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.h = srcRect.h * 2;
    destRect.w = srcRect.w * 2;
}

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}
