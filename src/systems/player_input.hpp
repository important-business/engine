#ifndef SYSTEMS_PLAYER_INPUT_HPP
#define SYSTEMS_PLAYER_INPUT_HPP

#include "systems.hpp"

#include "components/player.hpp"
#include "components/velocity.hpp"
#include "core/logging.hpp"

#include <anax/System.hpp>
#include <wink/signal.hpp>
#include <wink/slot.hpp>

namespace systems
{

// TODO(Keegan, Make systems classes)
struct PlayerInput : anax::System<anax::Requires<components::PlayerComponent,
                         components::PhysicsComponent>>
{
public:
    PlayerInput();
    void update();

    wink::signal<wink::slot<void (anax::Entity, float, float)>> m_movement_signal;
private:
    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* SYSTEMS_PLAYER_INPUT_HPP */
