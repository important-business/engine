#ifndef SYSTEMS_PLAYER_INPUT_HPP
#define SYSTEMS_PLAYER_INPUT_HPP

#include "components/player.hpp"
#include "components/velocity.hpp"

#include <anax/System.hpp>

namespace systems
{

struct PlayerInput : anax::System<anax::Requires<components::PlayerComponent,
                         components::VelocityComponent>>
{
public:
    void update();
};
}

#endif /* SYSTEMS_PLAYER_INPUT_HPP */
