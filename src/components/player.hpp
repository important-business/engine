#ifndef COMPONENTS_PLAYER_HPP
#define COMPONENTS_PLAYER_HPP

#include <anax/Component.hpp>
#include <SDL.h>

namespace components
{

struct PlayerComponent : anax::Component
{
    float base_speed = 1;

    struct Controls
    {
        uint8_t left;
        uint8_t right;
        uint8_t up;
        uint8_t down;

        Controls()
            : left(SDL_SCANCODE_LEFT),
              right(SDL_SCANCODE_RIGHT),
              up(SDL_SCANCODE_UP),
              down(SDL_SCANCODE_DOWN)
        {
        }

        Controls(uint8_t left, uint8_t right, uint8_t up, uint8_t down)
            : left(left), right(right), up(up), down(down)
        {
        }

    } controls;
};
}

#endif // COMPONENTS_PLAYER_HPP
