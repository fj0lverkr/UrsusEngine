#include "Camera2D.hpp"

Camera2D::Camera2D()
{
    this->viewFinder = { 0.0f, 0.0f, 1920, 1080 };
}

Camera2D::Camera2D(float x, float y, int w, int h)
{
    float vfWidth = static_cast<float>(w);
    float vfHeight = static_cast<float>(h);
    this->viewFinder = {x, y, vfWidth, vfHeight};
}

SDL_FRect Camera2D::GetViewFinder() const
{
    return this->viewFinder;
}

void Camera2D::follow(Vector2D &position, int windowWidth, int windowHeight)
{
    this->viewFinder.x = position.x - static_cast<float>(windowWidth) / 2;
    this->viewFinder.y = position.y - static_cast<float>(windowHeight) / 2;
    if (this->viewFinder.x < 0.0f)
    {
        this->viewFinder.x = 0.0f;
    }
    if (this->viewFinder.y < 0.0f)
    {
        this->viewFinder.y = 0.0f;
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
