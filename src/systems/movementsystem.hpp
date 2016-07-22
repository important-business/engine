#ifndef SYSTEMS_MOVEMENTSYSTEM_HPP
#define SYSTEMS_MOVEMENTSYSTEM_HPP

#include <anax/System.hpp>

#include "components/transform.hpp"
#include "components/velocity.hpp"

namespace systems
{

struct MovementSystem
    : anax::System<anax::Requires<components::TransformComponent,
          components::VelocityComponent>>
{
    void update(double delta_time);
};
}
#endif /* SYSTEMS_MOVEMENTSYSTEM_HPP */
