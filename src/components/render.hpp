#ifndef COMPONENTS_RENDER_HPP
#define COMPONENTS_RENDER_HPP

#include <anax/Component.hpp>
#include <sdl/wrap.hpp>

namespace components{

struct TextureComponent : anax::Component
{
    sdl::Texture *ptexture;
};

}

#endif /* COMPONENTS_RENDER_HPP */
