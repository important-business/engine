#ifndef SYSTEMS_RENDERSYSTEM_HPP
#define SYSTEMS_RENDERSYSTEM_HPP

#include <string>
#include <anax/System.hpp>
#include "components/render.hpp"
#include "components/transform.hpp"

namespace systems
{

struct RenderSystem : anax::System<anax::Requires<components::TextureComponent,
                          components::TransformComponent>>
{
    RenderSystem(sdl::Window* pwindowtouse, Uint32 renderflags);
    ~RenderSystem();

    void render();

    bool isValid() const;
    sdl::Renderer* getRenderer() const;
    sdl::Window* getWindow() const;

private:
    sdl::Window* pwindow;
    sdl::Renderer* prenderer;
};
}
#endif /* SYSTEMS_RENDERSYSTEM_HPP */
