#include "TextureManager.h"

SDL_Texture *TextureManager::LoadTexture(const char *fileName, SDL_Renderer *r)
{
    SDL_Surface *s = IMG_Load(fileName);
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    return t;
}