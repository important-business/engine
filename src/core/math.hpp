#ifndef MATH_HPP
#define MATH_HPP
namespace core{
struct Rectangle
{
    explicit Rectangle(float x = 0.0f, float y = 0.0f, float w=0.0f, float h=0.0f);

    float x;
    float y;
    float w;
    float h;
};

struct Vector
{
    explicit Vector(float x = 0.0f, float y = 0.0f);

    Vector operator+(const Vector& other);
    Vector &operator+=(const Vector& other);

    Vector operator-(const Vector& other);
    Vector &operator-=(const Vector& other);

    Vector operator*(const Vector& other);
    Vector &operator*=(const Vector& other);

    Vector operator/(const Vector& other);
    Vector &operator/=(const Vector& other);
    float dot(const Vector& other) const;

    float mag() const;

    float x;
    float y;
};

} // namespace core

#endif /* MATH_HPP */
