#ifndef COMPONENTS_RENDER_HPP
#define COMPONENTS_RENDER_HPP

#include <anax/Component.hpp>
#include <memory>
#include <sdl_wrap/wrap.hpp>
#include <string>

namespace components
{

extern const std::string str_name_texture;

struct TextureComponent : anax::Component
{
    TextureComponent(std::string texture_path) : texture_path(texture_path)
    {
    }

    const std::string texture_path;
    std::shared_ptr<sdl_wrap::Texture> sp_texture;
};
}

#endif /* COMPONENTS_RENDER_HPP */
