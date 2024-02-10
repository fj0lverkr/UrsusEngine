#include "Camera2D.hpp"

Camera2D::Camera2D()
{
}

Camera2D::Camera2D(int x, int y, int w, int h)
{
    this->viewFinder = {x, y, w, h};
}

SDL_Rect Camera2D::GetViewFinder()
{
    return this->viewFinder;
}

void Camera2D::follow(Vector2D &position, int windowWidth, int windowHeight)
{
    this->viewFinder.x = position.x - windowWidth / 2;
    this->viewFinder.y = position.y - windowHeight / 2;
    if (this->viewFinder.x < 0)
    {
        this->viewFinder.x = 0;
    }
    if (this->viewFinder.y < 0)
    {
        this->viewFinder.y = 0;
    }
    if (this->viewFinder.x > this->viewFinder.w)
    {
        this->viewFinder.x = this->viewFinder.w;
    }
    if (this->viewFinder.y > this->viewFinder.h)
    {
        this->viewFinder.y = this->viewFinder.h;
    }
}
