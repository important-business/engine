#ifndef COMPONENTS_COLLISION_HPP
#define COMPONENTS_COLLISION_HPP

#include <anax/Component.hpp>
#include <SDL.h>

namespace components
{

struct Collision : anax::Component
{
    SDL_Rect bounding_box;
    bool can_cause_events;
};
}

#endif /* COMPONENTS_COLLISION_HPP */
