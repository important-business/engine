#ifndef COMPONENTS_CAMERA_HPP
#define COMPONENTS_CAMERA_HPP

#include <anax/Component.hpp>
#include <anax/Entity.hpp>
#include <string>

namespace components
{

extern const std::string str_name_camera;

struct CameraComponent : anax::Component
{
    CameraComponent(float zoom) : zoom(zoom)
    {
    }
    float zoom;
};
}

#endif /* COMPONENTS_CAMERA_HPP */
