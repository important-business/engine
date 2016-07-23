#ifndef COMPONENTS_RENDER_HPP
#define COMPONENTS_RENDER_HPP

#include <memory>
#include <anax/Component.hpp>
#include <sdl_wrap/wrap.hpp>

namespace components
{

struct TextureComponent : anax::Component
{
    std::shared_ptr<sdl_wrap::Texture> p_texture;
};
}

#endif /* COMPONENTS_RENDER_HPP */
