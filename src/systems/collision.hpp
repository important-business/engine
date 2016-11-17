#ifndef SYSTEMS_COLLISION_HPP
#define SYSTEMS_COLLISION_HPP

#include <vector>
#include <algorithm>
#include <anax/System.hpp>
#include "core/logging.hpp"
#include "components/transform.hpp"
#include "components/collision.hpp"
#include "components/velocity.hpp"

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

    struct Listener
    {
        virtual ~Listener() = 0;

        virtual void on_collision_occured(
            anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold) = 0;
    };
    void update(double delta_time);
    void add_listener(Listener& listener);
    void remove_listener(Listener& listener);

private:
    Manifold* check_collision(anax::Entity& e1, anax::Entity& e2);
    void resolve_collision(
        anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold);
    std::vector<Listener*> m_listeners;
    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* SYSTEMS_COLLISION_HPP */
