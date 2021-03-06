/**
 * @file
 * @brief Common Math Header
 *
 * Defines common functions and classes for mathematical operations
 */

#ifndef MATH_HPP
#define MATH_HPP
namespace common{
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

    friend bool operator==(const Vector &v1, const Vector &v2);

    friend bool operator!=(const Vector &v1, const Vector &v2);

    friend bool operator>(const Vector &v1, const Vector &v2);

    friend bool operator<(const Vector &v1, const Vector &v2);

    float dot(const Vector& other) const;

    float mag() const;

    Vector normalize() const;

    float x;
    float y;
};

} // namespace common

#endif /* MATH_HPP */
