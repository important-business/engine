#ifndef MATH_HPP
#define MATH_HPP
namespace core{
struct Vector
{
    explicit Vector(float x = 0.0f, float y = 0.0f);

    Vector operator+(const Vector& other);

    Vector operator-(const Vector& other);

    Vector operator*(const Vector& other);

    float dot(const Vector& other);

    float x;
    float y;
};

} // namespace core

#endif /* MATH_HPP */
