#ifndef SYSTEMS_RENDERSYSTEM_HPP
#define SYSTEMS_RENDERSYSTEM_HPP

#include <string>
#include <anax/System.hpp>
#include "components/render.hpp"
#include "components/transform.hpp"

namespace systems
{

struct Render : anax::System<anax::Requires<components::TextureComponent,
                    components::TransformComponent>>
{
    Render(sdl_wrap::Window* p_window, Uint32 render_flags);
    ~Render();

    void render();

    bool is_valid() const;

    sdl_wrap::Renderer* get_renderer() const;

    sdl_wrap::Window* get_window() const;

private:
    sdl_wrap::Window* m_p_window;

    sdl_wrap::Renderer* m_p_renderer;
};
}
#endif /* SYSTEMS_RENDERSYSTEM_HPP */
