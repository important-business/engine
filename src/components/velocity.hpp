#ifndef COMPONENTS_VELOCITY_HPP
#define COMPONENTS_VELOCITY_HPP

#include <anax/Component.hpp>

namespace components
{

struct VelocityComponent : anax::Component
{
    VelocityComponent(float x = 0.0f, float y = 0.0f) : x(x), y(y)
    {
    }
    float x;
    float y;
};
}

#endif /* COMPONENTS_VELOCITY_HPP */
