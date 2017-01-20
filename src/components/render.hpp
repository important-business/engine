/**
 * @file
 * @brief Render Component Header
 *
 * Contains definition of components relating to Rendering
 */

#ifndef COMPONENTS_RENDER_HPP
#define COMPONENTS_RENDER_HPP

#include <anax/Component.hpp>
#include <memory>
#include <sdl_wrap/wrap.hpp>
#include <string>

namespace components
{

struct TextureComponent : anax::Component
{
    explicit TextureComponent(std::string texture_path) : texture_path(texture_path)
    {
    }

    const std::string texture_path;
    std::shared_ptr<sdl_wrap::Texture> sp_texture;


    static const std::string name;
};
}

#endif /* COMPONENTS_RENDER_HPP */
