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

        velocity.velocity.x += velocity.force.x * velocity.inv_mass;
        velocity.velocity.y += velocity.force.y * velocity.inv_mass;
        velocity.force.x = 0.0f;
        velocity.force.y = 0.0f;

        // Mass increase of normal force and reduction in delta velocity cancel
        if (velocity.velocity.x > 0.0f)
        {
            velocity.velocity.x -= velocity.friction;
        }
        else if (velocity.velocity.x < 0.0f)
        {
            velocity.velocity.x += velocity.friction;
        }

        if (velocity.velocity.y > 0.0f)
        {
            velocity.velocity.y -= velocity.friction;
        }
        else if (velocity.velocity.y < 0.0f)
        {
            velocity.velocity.y += velocity.friction;
        }

        if (abs(velocity.velocity.y) > m_min_vel)
        {
            auto distance_moved_y =
                velocity.velocity.y * static_cast<float>(delta_time);
            transform.pos_y += distance_moved_y;
        }
        else
        {
            velocity.velocity.y = 0.0f;
        }

        if (abs(velocity.velocity.x) > m_min_vel)
        {
            auto distance_moved_x =
                velocity.velocity.x * static_cast<float>(delta_time);
            transform.pos_x += distance_moved_x;
        }
        else
        {
            velocity.velocity.x = 0.0f;
        }
    }
}

} // namespace systems
