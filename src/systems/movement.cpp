#include "systems/movement.hpp"
#include "components/id.hpp"
#include "common/exception.hpp"

#include <cmath>
#include <iostream>

namespace systems
{

Movement::Movement()
{
    m_sp_logger = core::logging_get_logger("movement");
}

void Movement::move_actor(anax::Entity entity, float vel_x, float vel_y)
{
    throw_if_missing_component<components::MovementComponent>(entity);
    throw_if_missing_component<components::PhysicsComponent>(entity);

    auto& movement = entity.getComponent<components::MovementComponent>();
    const float top_speed = movement.top_speed;
    const float accel = movement.accel;
    auto& physics = entity.getComponent<components::PhysicsComponent>();

    float vel_error_x = (vel_x * top_speed) - (physics.velocity.x);
    float vel_error_y = (vel_y * top_speed) - (physics.velocity.y);

    float vel_factor_x = vel_error_x / top_speed;
    float vel_factor_y = vel_error_y / top_speed;

    physics.force.x += accel * vel_factor_x;
    physics.force.y += accel * vel_factor_y;
}

void Movement::update(double delta_time)
{
    auto entities = getEntities();
    for (auto& entity : entities)
    {
        // No hasComponent check necessary since getEntities filters
        // already
        auto& transform = entity.getComponent<components::TransformComponent>();
        auto& physics = entity.getComponent<components::PhysicsComponent>();

        physics.velocity.x += physics.force.x * physics.inv_mass;
        physics.velocity.y += physics.force.y * physics.inv_mass;
        physics.force.x = 0.0f;
        physics.force.y = 0.0f;

        // Mass increase of normal force and reduction in delta velocity cancel
        if (physics.velocity.x > 0.0f)
        {
            physics.velocity.x -= physics.friction;
        }
        else if (physics.velocity.x < 0.0f)
        {
            physics.velocity.x += physics.friction;
        }

        if (physics.velocity.y > 0.0f)
        {
            physics.velocity.y -= physics.friction;
        }
        else if (physics.velocity.y < 0.0f)
        {
            physics.velocity.y += physics.friction;
        }

        if (std::abs(physics.velocity.y) > m_min_vel)
        {
            auto distance_moved_y =
                physics.velocity.y * static_cast<float>(delta_time);
            transform.pos_y += distance_moved_y;
        }
        else
        {
            physics.velocity.y = 0.0f;
        }

        if (std::abs(physics.velocity.x) > m_min_vel)
        {
            auto distance_moved_x =
                physics.velocity.x * static_cast<float>(delta_time);
            transform.pos_x += distance_moved_x;
        }
        else
        {
            physics.velocity.x = 0.0f;
        }
    }
}

} // namespace systems
