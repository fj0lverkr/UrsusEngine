#include "TextureManager.h"

SDL_Texture *TextureManager::LoadTexture(const char *fileName)
{
    SDL_Surface *s = IMG_Load(fileName);
    SDL_Texture *t = SDL_CreateTextureFromSurface(Game::renderer, s);
    SDL_FreeSurface(s);
    return t;
}

void TextureManager::Draw(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}
