#ifndef SYSTEMS_PLAYER_INPUT_HPP
#define SYSTEMS_PLAYER_INPUT_HPP

#include "components/player.hpp"
#include "components/velocity.hpp"
#include "core/logging.hpp"

#include <anax/System.hpp>

namespace systems
{

struct PlayerInput : anax::System<anax::Requires<components::PlayerComponent,
                         components::VelocityComponent>>
{
public:
    PlayerInput();
    void update();

private:
    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* SYSTEMS_PLAYER_INPUT_HPP */
