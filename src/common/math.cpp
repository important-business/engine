#include "common/math.hpp"
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

bool operator==(const Vector& v1, const Vector& v2)
{
    return v1.mag() == v2.mag();
}

bool operator!=(const Vector& v1, const Vector& v2)
{
    return !(v1 == v2);
}

bool operator>(const Vector& v1, const Vector& v2)
{
    return v1.mag() > v2.mag();
}

bool operator<(const Vector& v1, const Vector& v2)
{
    return v1.mag() > v2.mag();
}

float Vector::dot(const Vector& other) const
{
    return other.x * x + other.y * y;
}

float Vector::mag() const
{
    return sqrt(x * x + y * y);
}

Vector Vector::normalize() const
{
    auto magnitude = mag();
    if (magnitude != 0.0f)
    {
        auto norm_x = x / magnitude;
        auto norm_y = y / magnitude;
        return Vector(norm_x, norm_y);
    }
    else
    {
        return Vector(0.0f, 0.0f);
    }
}

} // namespace core
