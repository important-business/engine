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
    VelocityComponent(float inertia,
        float friction,
        float vel_x,
        float vel_y,
        float force_x,
        float force_y)
        : inertia(inertia), friction(friction)
    {
        velocity.x = vel_x;
        velocity.y = vel_y;
        force.x = force_x;
        force.y = force_y;
    }
    float inertia;
    float friction;
    Vector velocity;
    Vector force;
};
}

#endif /* COMPONENTS_VELOCITY_HPP */
