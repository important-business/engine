#ifndef SYSTEMS_COLLISION_HPP
#define SYSTEMS_COLLISION_HPP

#include "systems.hpp"

#include "core/logging.hpp"
#include "components/collision.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"

#include <vector>
#include <algorithm>
#include <anax/System.hpp>
#include <wink/signal.hpp>
#include <wink/slot.hpp>

namespace systems
{

struct Manifold
{
    components::Vector normal = {0.0f, 0.0f};
    components::Vector penetration = {0.0f, 0.0f};
};

struct Collision
    : anax::System<
          anax::Requires<components::TransformComponent, components::Collision>>
{
public:
    Collision();

    void update(double delta_time);

    wink::signal<wink::slot<void (std::string, anax::Entity)>> m_trigger_signal;

private:
    Manifold* check_collision(anax::Entity& e1, anax::Entity& e2);
    void resolve_collision(
        anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold);
    void check_trigger(
        anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold);

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* SYSTEMS_COLLISION_HPP */
