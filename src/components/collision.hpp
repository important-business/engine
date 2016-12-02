#ifndef COMPONENTS_COLLISION_HPP
#define COMPONENTS_COLLISION_HPP

#include <anax/Component.hpp>
#include <SDL.h>
#include <string>

namespace components
{

struct Collision : anax::Component
{
    Collision(int x, int y, int w, int h, bool can_cause_events)
        : can_cause_events(can_cause_events)
    {
        bounding_box.x = x;
        bounding_box.y = y;
        bounding_box.h = h;
        bounding_box.w = w;
    }
    SDL_Rect bounding_box;
    bool can_cause_events;

    static const std::string name;
};
}

#endif /* COMPONENTS_COLLISION_HPP */
