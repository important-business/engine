#include "systems/render.hpp"

namespace systems
{

Render::Render(sdl_wrap::Window* p_window, Uint32 render_flags)
{
    m_p_window = p_window;

    m_p_renderer = new sdl_wrap::Renderer{*m_p_window, -1, render_flags};
    m_p_renderer->set_draw_color(150, 150, 150, 100);

    m_sp_logger = core::logging_get_logger("render");
}

void Render::render(core::Level* plevel)
{
    m_p_renderer->clear();
    render_level(plevel);
    render_entities();
    m_p_renderer->present();
}

void Render::render_entities()
{
    auto entities = getEntities();

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
}

void Render::render_level(core::Level* plevel)
{
    uint16_t size_x, size_y;
    plevel->get_size(size_x, size_y);
    auto scale = plevel->get_scale();
    // TODO: Add some smartness to clip out irrelevant portions of the level
    for (uint16_t pos_y = 0; pos_y < size_y; pos_y++)
    {
        for (uint16_t pos_x = 0; pos_x < size_x; pos_x++)
        {
            SDL_Rect dest_rect = {static_cast<int>(pos_x * scale),
                static_cast<int>(pos_y * scale),
                static_cast<int>((pos_x + 1) * scale),
                static_cast<int>((pos_y + 1) * scale)};
            uint8_t red, blue, green;

            auto ptile = plevel->get(pos_x, pos_y);
            ptile->get_color(red, blue, green);
            m_p_renderer->set_draw_color(red, blue, green, 255);
            m_p_renderer->fill_rect(&dest_rect);
        }
    }
}

sdl_wrap::Renderer* Render::get_renderer() const
{
    return m_p_renderer;
}

sdl_wrap::Window* Render::get_window() const
{
    return m_p_window;
}

} // namespace systems
