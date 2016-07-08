#ifndef COMPONENTS_TRANSFORM_HPP
#define COMPONENTS_TRANSFORM_HPP

#include <anax/Component.hpp>

namespace components{

struct TransformComponent : anax::Component
{
    TransformComponent(float pos_x, float pos_y, float size_x, float size_y, float rotation, bool flip_vert, bool flip_horiz):
        pos_x(pos_x), pos_y(pos_y), size_x(size_x), size_y(size_y), rotation(rotation), flip_vert(flip_vert), flip_horiz(flip_horiz) {}
    // TODO: Decide if floats are appropriate, or if we want ints
    float pos_x;
    float pos_y;
    float size_x;
    float size_y;
    float rotation;
    bool flip_vert;
    bool flip_horiz;
};

}

#endif /* COMPONENTS_TRANSFORM_HPP */
