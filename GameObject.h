#include <SDL2/SDL.h>

class GameObject
{
private:
    int xpos;
    int ypos;
    SDL_Texture *objTexture;
    SDL_Rect srcRect, destRect;
    SDL_Renderer *renderer;

public:
    GameObject(const char *textureSheet, SDL_Renderer *r, int x, int y, int srcX, int srcY, int srcW, int srcH);
    ~GameObject();

    void Update();
    void Render();
};