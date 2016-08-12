#ifndef SYSTEMS_MOVEMENTSYSTEM_HPP
#define SYSTEMS_MOVEMENTSYSTEM_HPP

#include "components/transform.hpp"
#include "components/velocity.hpp"

#include <anax/System.hpp>

namespace systems
{

struct Movement : anax::System<anax::Requires<components::TransformComponent,
                      components::VelocityComponent>>
{
    void update(double delta_time);
};
}
#endif /* SYSTEMS_MOVEMENTSYSTEM_HPP */
