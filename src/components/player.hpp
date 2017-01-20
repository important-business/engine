/**
 * @file
 * @brief Player Component Header
 *
 * Contains definition of components relating to player
 */

#ifndef COMPONENTS_PLAYER_HPP
#define COMPONENTS_PLAYER_HPP

#include <anax/Component.hpp>
#include <SDL.h>
#include <string>

namespace components
{

extern const std::string str_name_player;

struct PlayerComponent : anax::Component
{
    PlayerComponent()
    {
    }

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

    static const std::string name;
};
}

#endif /* COMPONENTS_PLAYER_HPP */
