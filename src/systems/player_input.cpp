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
            velocity.x = -player.base_speed;
        }
        else if (static_cast<bool>(key_states[player.controls.right]))
        {
            velocity.x = player.base_speed;
        }
        else
        {
            velocity.x = 0;
        }

        if (static_cast<bool>(key_states[player.controls.up]))
        {
            velocity.y = -player.base_speed;
        }
        else if (static_cast<bool>(key_states[player.controls.down]))
        {
            velocity.y = player.base_speed;
        }
        else
        {
            velocity.y = 0;
        }
    }
}

} // namespace systems
