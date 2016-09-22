#ifndef SYSTEMS_COLLISION_HPP
#define SYSTEMS_COLLISION_HPP

#include <anax/System.hpp>
#include "components/transform.hpp"
#include "components/collision.hpp"

namespace systems
{
struct Collision : anax::System<anax::Requires<components::TransformComponent>>
{
public:
    struct Listener
    {
        virtual ~Listener() = 0;

        virtual void on_collision_occured(
            anax::Entity& e1, anax::Entity& e2) = 0;
    };
    void update(double delta_time);
    void add_listener(Listener& listener);
    void remove_listener(Listener& listener);

private:
    std::vector<Listener*> m_listeners;
};
}

#endif /* SYSTEMS_COLLISION_HPP */
