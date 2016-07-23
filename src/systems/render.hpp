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
    RenderSystem(sdl::Window* p_window, Uint32 render_flags);

    ~RenderSystem();

    void render();

    bool is_valid() const;

    sdl::Renderer* get_renderer() const;

    sdl::Window* get_window() const;

private:
    sdl::Window* m_p_window;

    sdl::Renderer* m_p_renderer;
};
}
#endif /* SYSTEMS_RENDERSYSTEM_HPP */
