#ifndef COMPONENTS_VELOCITY_HPP
#define COMPONENTS_VELOCITY_HPP

#include "common/math.hpp"
#include <anax/Component.hpp>
#include <string>

namespace components
{

struct MovementComponent : anax::Component
{
public:
    MovementComponent(float accel, float top_speed)
        : accel(accel), top_speed(top_speed){}
    float accel;
    float top_speed;

    static const std::string name;
};

struct PhysicsComponent : anax::Component
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
    PhysicsComponent(float mass,
        float friction,
        float restitution,
        float vel_x,
        float vel_y,
        float force_x,
        float force_y)
        : inv_mass(calc_inv_mass(mass)), friction(friction), restitution(restitution)
    {
        velocity.x = vel_x;
        velocity.y = vel_y;
        force.x = force_x;
        force.y = force_y;
    }
    float inv_mass;
    float friction;
    float restitution;
    common::Vector velocity;
    common::Vector force;

    static const std::string name;
};
}

#endif /* COMPONENTS_VELOCITY_HPP */
