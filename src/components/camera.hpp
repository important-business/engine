#ifndef COMPONENTS_CAMERA_HPP
#define COMPONENTS_CAMERA_HPP

#include <anax/Component.hpp>
#include <anax/Entity.hpp>

namespace components
{

struct CameraComponent : anax::Component
{
    CameraComponent(anax::Entity target, float zoom)
        : target(target), zoom(zoom)
    {
    }
    anax::Entity target;
    float zoom;
};
}

#endif /* COMPONENTS_CAMERA_HPP */
