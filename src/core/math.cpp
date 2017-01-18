#include "core/math.hpp"
#include <cmath>

namespace core
{

Rectangle::Rectangle(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h)
{
}

Vector::Vector(float x, float y) : x(x), y(y)
{
}

Vector Vector::operator+(const Vector& other)
{
    float res_x = x + other.x;
    float res_y = y + other.y;
    return Vector(res_x, res_y);
}

Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    return (*this);
}

Vector Vector::operator-(const Vector& other)
{
    float res_x = x - other.x;
    float res_y = y - other.y;
    return Vector(res_x, res_y);
}

Vector& Vector::operator-=(const Vector& other)
{
    x -= other.x;
    y -= other.y;
    return (*this);
}

Vector Vector::operator*(const Vector& other)
{
    float res_x = x * other.x;
    float res_y = y * other.y;
    return Vector(res_x, res_y);
}

Vector& Vector::operator*=(const Vector& other)
{
    x *= other.x;
    y *= other.y;
    return (*this);
}

Vector Vector::operator/(const Vector& other)
{
    float res_x = x / other.x;
    float res_y = y / other.y;
    return Vector(res_x, res_y);
}

Vector& Vector::operator/=(const Vector& other)
{
    x /= other.x;
    y /= other.y;
    return (*this);
}

float Vector::dot(const Vector& other) const
{
    return other.x * x + other.y * y;
}

float Vector::mag() const
{
    return sqrt(x * x + y * y);
}

} // namespace core
