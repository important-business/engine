#ifndef PLAYERINPUTSYSTEM_HPP
#define PLAYERINPUTSYSTEM_HPP

#include <anax/System.hpp>
#include <components/player.hpp>
#include <components/velocity.hpp>

namespace systems
{

struct PlayerInput : anax::System<anax::Requires<components::PlayerComponent,
                         components::VelocityComponent>>
{
public:
    void update(double delta_time);
};
}

#endif // PLAYERINPUTSYSTEM_HPP
