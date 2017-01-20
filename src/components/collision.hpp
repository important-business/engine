#ifndef COMPONENTS_COLLISION_HPP
#define COMPONENTS_COLLISION_HPP

#include "common/math.hpp"
#include <anax/Component.hpp>
#include <string>


namespace components
{

struct Collision : anax::Component
{
    Collision(float x, float y, float w, float h, bool can_cause_events)
        : can_cause_events(can_cause_events)
    {
        bounding_box.x = x;
        bounding_box.y = y;
        bounding_box.h = h;
        bounding_box.w = w;
    }
    common::Rectangle bounding_box;
    bool can_cause_events;

    static const std::string name;
};

struct Trigger : anax::Component
{
    explicit Trigger( std::string trigger_msg)
    :trigger_msg(trigger_msg){}

    std::string trigger_msg;

    static const std::string name;
};

}

#endif /* COMPONENTS_COLLISION_HPP */
