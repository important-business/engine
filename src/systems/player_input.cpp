#include "systems/player_input.hpp"

#include <SDL.h>

namespace systems
{

PlayerInput::PlayerInput()
{
    m_sp_logger = core::logging_get_logger("playerinput");
}

void PlayerInput::update()
{
    auto entities = getEntities();
    for (auto e : entities)
    {
        auto& player = e.getComponent<components::PlayerComponent>();
        auto& velocity = e.getComponent<components::VelocityComponent>();

        const auto& key_states = SDL_GetKeyboardState(nullptr);
        if (static_cast<bool>(key_states[player.controls.left]))
        {
            if (velocity.velocity.x > -player.top_speed)
            {
                velocity.force.x += -player.move_accel;
            }
        }
        else if (static_cast<bool>(key_states[player.controls.right]))
        {
            if (velocity.velocity.x < player.top_speed)
            {
                velocity.force.x += player.move_accel;
            }
        }

        if (static_cast<bool>(key_states[player.controls.up]))
        {
            if (velocity.velocity.y > -player.top_speed)
            {
                velocity.force.y += -player.move_accel;
            }
        }
        else if (static_cast<bool>(key_states[player.controls.down]))
        {
            if (velocity.velocity.y < player.top_speed)
            {
                velocity.force.y += player.move_accel;
            }
        }
    }
}

} // namespace systems
