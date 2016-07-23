#include "systems/render.hpp"

#include <iostream>

namespace systems
{

RenderSystem::RenderSystem(sdl::Window* p_window, Uint32 render_flags)
{
    m_p_window = p_window;

    m_p_renderer = new sdl::Renderer{*m_p_window, -1, render_flags};
    m_p_renderer->set_draw_color(150, 150, 150, 100);
}
RenderSystem::~RenderSystem()
{
}

void RenderSystem::render()
{
    auto entities = getEntities();

    m_p_renderer->clear();
    for (auto& entity : entities)
    {
        auto texture_component =
            entity.getComponent<components::TextureComponent>();
        auto transform_component =
            entity.getComponent<components::TransformComponent>();
        SDL_Rect dest_rect = {
            static_cast<int>(transform_component.pos_x),
            static_cast<int>(transform_component.pos_y),
            static_cast<int>(transform_component.size_x),
            static_cast<int>(transform_component.size_y),
        };
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (transform_component.flip_vert)
        {
            flip = static_cast<SDL_RendererFlip>(
                static_cast<SDL_RendererFlip>(SDL_FLIP_VERTICAL) | flip);
        }
        if (transform_component.flip_horiz)
        {
            flip = static_cast<SDL_RendererFlip>(
                static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL) | flip);
        }
        m_p_renderer->copy(*texture_component.p_texture,
            nullptr,
            &dest_rect,
            static_cast<int>(transform_component.rotation),
            flip);
    }
    m_p_renderer->present();
}

sdl::Renderer* RenderSystem::get_renderer() const
{
    return m_p_renderer;
}

sdl::Window* RenderSystem::get_window() const
{
    return m_p_window;
}

} // namespace systems
