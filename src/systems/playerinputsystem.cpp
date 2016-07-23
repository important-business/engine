#include <SDL.h>
#include <systems/playerinputsystem.hpp>

using namespace systems;

void PlayerInputSystem::update(double delta_time)
{
    auto entities = getEntities();
    for (auto e : entities)
    {
        auto& player = e.getComponent<components::PlayerComponent>();
        auto& velocity = e.getComponent<components::VelocityComponent>();

        const auto& key_states = SDL_GetKeyboardState(nullptr);
        if (key_states[player.controls.left])
        {
            velocity.x = -player.base_speed;
        }
        else if (key_states[player.controls.right])
        {
            velocity.x = player.base_speed;
        }
        else
        {
            velocity.x = 0;
        }

        if (key_states[player.controls.up])
        {
            velocity.y = -player.base_speed;
        }
        else if (key_states[player.controls.down])
        {
            velocity.y = player.base_speed;
        }
        else
        {
            velocity.y = 0;
        }
    }
}
