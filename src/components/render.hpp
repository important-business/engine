#ifndef COMPONENTS_RENDER_HPP
#define COMPONENTS_RENDER_HPP

#include <anax/Component.hpp>
#include <memory>
#include <sdl_wrap/wrap.hpp>

namespace components
{

struct TextureComponent : anax::Component
{
    std::shared_ptr<sdl_wrap::Texture> p_texture;
};
}

#endif /* COMPONENTS_RENDER_HPP */
