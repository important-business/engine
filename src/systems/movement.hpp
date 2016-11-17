#ifndef SYSTEMS_MOVEMENT_HPP
#define SYSTEMS_MOVEMENT_HPP

#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "core/logging.hpp"

#include <anax/System.hpp>

namespace systems
{

// TODO(Keegan, Make this a class)
struct Movement : anax::System<anax::Requires<components::TransformComponent,
                      components::VelocityComponent>>
{
public:
    Movement();
    void update(double delta_time);

private:
    std::shared_ptr<spdlog::logger> m_sp_logger;
    const float m_min_vel = 0.001f;
};
}
#endif /* SYSTEMS_MOVEMENT_HPP */
