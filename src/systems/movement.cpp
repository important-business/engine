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

        auto distance_moved_x = velocity.x * static_cast<float>(delta_time);
        auto distance_moved_y = velocity.y * static_cast<float>(delta_time);

        transform.pos_x += distance_moved_x;
        transform.pos_y += distance_moved_y;
    }
}

} // namespace systems
