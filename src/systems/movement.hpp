/**
 * @file
 * @brief System Movement Header
 *
 * Contains definition of classes for Movement Logic
 */

#ifndef SYSTEMS_MOVEMENT_HPP
#define SYSTEMS_MOVEMENT_HPP

#include "systems.hpp"

#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "common/logging.hpp"

#include <anax/System.hpp>

namespace systems
{

// TODO(Keegan): Make this a class)
struct Movement : anax::System<anax::Requires<components::TransformComponent,
                      components::PhysicsComponent>>
{
public:
    Movement();
    void move_actor(anax::Entity entity, float vel_x, float vel_y);
    void update(double delta_time);

private:
    const float m_min_vel = 0.001f;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}
#endif /* SYSTEMS_MOVEMENT_HPP */
