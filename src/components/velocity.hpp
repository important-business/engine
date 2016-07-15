#ifndef COMPONENTS_MOVEMENT_HPP
#define COMPONENTS_MOVEMENT_HPP

#include <anax/Component.hpp>

namespace components
{

struct VelocityComponent : anax::Component
{
    float x;
    float y;
};
}

#endif /* COMPONENTS_MOVEMENT_HPP */
