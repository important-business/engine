#ifndef SYSTEMS_COLLISION_HPP
#define SYSTEMS_COLLISION_HPP

#include "systems.hpp"

#include "common/logging.hpp"
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
    common::Vector normal;
    common::Vector penetration;

    explicit operator bool();
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
    Manifold check_rect_collision(const common::Rectangle &rect1, const common::Rectangle &rect2);
    Manifold check_collision(anax::Entity& e1, anax::Entity& e2);
    Manifold check_level_collision(anax::Entity& e, core::Level *p_level);
    void resolve_collision(
        anax::Entity& e1, anax::Entity& e2, const Manifold& manifold);
    void resolve_collision(
        anax::Entity& e1, const Manifold& manifold);
    void do_resolve_collision(components::PhysicsComponent *p_physics1,
            components::TransformComponent *p_transform1,
            components::PhysicsComponent *p_physics2,
            components::TransformComponent *p_transform2,
            const Manifold& manifold);
    void check_trigger(
        anax::Entity& e1, anax::Entity& e2, const Manifold& manifold);

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* SYSTEMS_COLLISION_HPP */
