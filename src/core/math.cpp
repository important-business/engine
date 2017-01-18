#include "core/math.hpp"
#include <cmath>

namespace core
{

Vector::Vector(float x, float y) : x(x), y(y)
{
}

Vector Vector::operator+(const Vector& other)
{
    float res_x = x + other.x;
    float res_y = y + other.y;
    return Vector(res_x, res_y);
}

Vector Vector::operator-(const Vector& other)
{
    float res_x = x - other.x;
    float res_y = y - other.y;
    return Vector(res_x, res_y);
}

Vector Vector::operator*(const Vector& other)
{
    float res_x = x * other.x;
    float res_y = y * other.y;
    return Vector(res_x, res_y);
}

float Vector::dot(const Vector& other)
{
    return other.x * x + other.y * y;
}

float Vector::mag()
{
    return sqrt(x * x + y * y);
}

} // namespace core
