#include "TextureManager.h"
#include "Game.h"

SDL_Texture *TextureManager::LoadTexture(const char *fileName)
{
    SDL_Surface *s = IMG_Load(fileName);
    SDL_Texture *t = SDL_CreateTextureFromSurface(Game::renderer, s);
    SDL_FreeSurface(s);
    return t;
}