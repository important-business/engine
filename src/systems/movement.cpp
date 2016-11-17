#include "systems/movement.hpp"

#include <iostream>

namespace systems
{

Movement::Movement()
{
    m_sp_logger = core::logging_get_logger("movement");
}

void Movement::update(double delta_time)
{
    auto entities = getEntities();
    for (auto& entity : entities)
    {
        auto& transform = entity.getComponent<components::TransformComponent>();
        auto& velocity = entity.getComponent<components::VelocityComponent>();

        velocity.velocity.x +=
            velocity.force.x * static_cast<float>(delta_time);
        velocity.velocity.y +=
            velocity.force.y * static_cast<float>(delta_time);
        velocity.force.x = 0;
        velocity.force.y = 0;
        auto distance_moved_x =
            velocity.velocity.x * static_cast<float>(delta_time);
        auto distance_moved_y =
            velocity.velocity.y * static_cast<float>(delta_time);

        transform.pos_x += distance_moved_x;
        transform.pos_y += distance_moved_y;
    }
}

} // namespace systems
