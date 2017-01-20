/**
 * @file
 * @brief Camera Component Header
 *
 * Contains definition of components relating to the Camera
 */

#ifndef COMPONENTS_CAMERA_HPP
#define COMPONENTS_CAMERA_HPP

#include <anax/Component.hpp>
#include <anax/Entity.hpp>
#include <string>

namespace components
{

struct CameraComponent : anax::Component
{
    explicit CameraComponent(float zoom) : zoom(zoom)
    {
    }
    float zoom;

    static const std::string name;
};
}

#endif /* COMPONENTS_CAMERA_HPP */
