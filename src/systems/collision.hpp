#ifndef SYSTEMS_COLLISION_HPP
#define SYSTEMS_COLLISION_HPP

#include "systems.hpp"

#include "core/logging.hpp"
#include "core/level.hpp"
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
public:
    Manifold(){
        normal.x = 0.0f;
        normal.y = 0.0f;
        penetration.x = 0.0f;
        penetration.y = 0.0f;
    }
    core::Vector normal;
    core::Vector penetration;
};

struct Collision
    : anax::System<
          anax::Requires<components::TransformComponent, components::Collision>>
{
public:
    Collision();

    void update(core::Level *p_level = nullptr);

    wink::signal<wink::slot<void (std::string, anax::Entity)>> m_trigger_signal;

private:
    Manifold* check_collision(anax::Entity& e1, anax::Entity& e2);
    Manifold* check_level_collision(anax::Entity& e, core::Level *p_level);
    Manifold* do_check_collision(anax::Entity& e1, anax::Entity& e2);
    void resolve_collision(
        anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold);
    void resolve_collision(
        anax::Entity& e1, Manifold* p_manifold);
    void do_resolve_collision(components::PhysicsComponent *p_physics1,
            components::TransformComponent *p_transform1,
            components::PhysicsComponent *p_physics2,
            components::TransformComponent *p_transform2,
            Manifold* p_manifold);
    void check_trigger(
        anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold);

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* SYSTEMS_COLLISION_HPP */
