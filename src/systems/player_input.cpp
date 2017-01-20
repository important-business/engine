/**
 * @file
 * @brief System Player Input Source
 *
 * Contains implementation of classes for input logic
 */

#include "systems/player_input.hpp"

#include <SDL.h>

namespace systems
{

PlayerInput::PlayerInput()
    : m_sp_logger(common::logging_get_logger("playerinput"))
{
}

void PlayerInput::update()
{
    auto entities = getEntities();
    for (auto e : entities)
    {
        float vel_x = 0.0f, vel_y = 0.0f;
        auto& player = e.getComponent<components::PlayerComponent>();

        const auto& key_states = SDL_GetKeyboardState(nullptr);
        if (static_cast<bool>(key_states[player.controls.left]))
        {
            vel_x = -1.0f;
        }
        else if (static_cast<bool>(key_states[player.controls.right]))
        {
            vel_x = 1.0f;
        }

        if (static_cast<bool>(key_states[player.controls.up]))
        {
            vel_y = -1.0f;
        }
        else if (static_cast<bool>(key_states[player.controls.down]))
        {
            vel_y = 1.0f;
        }

        m_movement_signal.emit(e, vel_x, vel_y);
    }
}

} // namespace systems
