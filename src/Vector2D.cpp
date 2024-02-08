#include "Vector2D.h"
#include <math.h>

Vector2D::Vector2D()
{
    x = y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D &Vector2D::Zero()
{
    this->x = this->y = 0.0f;
    return *this;
}

bool Vector2D::IsZero()
{
    return this->x == 0.0f && this->y == 0.0f;
}

Vector2D &Vector2D::Normalize()
{
    if (this->x != 0 && this->y != 0)
    {
        float L = sqrtf(this->x * this->x + this->y * this->y);
        this->x /= L;
        this->y /= L;
    }

    return *this;
}

Vector2D &Vector2D::Add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;

    return *this;
}

Vector2D &Vector2D::Subtract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;

    return *this;
}

Vector2D &Vector2D::Multiply(const Vector2D &vec)
{
    this->x *= vec.x;
    this->y *= vec.y;

    return *this;
}

Vector2D &Vector2D::Divide(const Vector2D &vec)
{
    this->x /= vec.x;
    this->y /= vec.y;

    return *this;
}

Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
    return this->Add(vec);
}

Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
    return this->Subtract(vec);
}

Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
    return this->Multiply(vec);
}

Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
    return this->Divide(vec);
}

Vector2D &Vector2D::operator*(const int &i)
{
    this->x *= i;
    this->y *= i;

    return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2)
{
    return v1.Add(v2);
}

Vector2D &operator-(Vector2D &v1, const Vector2D &v2)
{
    return v1.Subtract(v2);
}

Vector2D &operator*(Vector2D &v1, const Vector2D &v2)
{
    return v1.Multiply(v2);
}

Vector2D &operator/(Vector2D &v1, const Vector2D &v2)
{
    return v1.Divide(v2);
}

std::ostream &operator<<(std::ostream &stream, const Vector2D &vec)
{
    stream << "Vector2D(" << vec.x << ", " << vec.y << ")";
    return stream;
}
