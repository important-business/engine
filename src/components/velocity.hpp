#ifndef COMPONENTS_VELOCITY_HPP
#define COMPONENTS_VELOCITY_HPP

#include <anax/Component.hpp>

namespace components
{

struct Vector
{
    Vector(float x = 0.0f, float y = 0.0f) : x(x), y(y)
    {
    }
    float x;
    float y;
};

struct VelocityComponent : anax::Component
{
private:
    static float calc_inv_mass(float mass)
    {
        float inv_mass;
        if (mass == 0.0f)
        {
            inv_mass = 0.0f;
        }
        else
        {
            inv_mass = 1.0f / mass;
        }
        return inv_mass;
    }

public:
    VelocityComponent(float mass,
        float friction,
        float vel_x,
        float vel_y,
        float force_x,
        float force_y)
        : inv_mass(calc_inv_mass(mass)), friction(friction)
    {
        velocity.x = vel_x;
        velocity.y = vel_y;
        force.x = force_x;
        force.y = force_y;
    }
    float inv_mass;
    float friction;
    Vector velocity;
    Vector force;
};
}

#endif /* COMPONENTS_VELOCITY_HPP */
