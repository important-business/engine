#include "systems/render.hpp"

namespace systems
{

void Camera::update()
{
    // TODO(Keegan): Update the camera's movement position based on target
}

Render::Render(
    sdl_wrap::Window* p_window, Uint32 render_flags, Camera& camerasystem)
    : m_camerasystem(camerasystem)
{
    m_p_window = p_window;

    m_p_renderer = new sdl_wrap::Renderer{*m_p_window, -1, render_flags};
    m_p_renderer->set_draw_color(150, 150, 150, 100);

    m_sp_logger = core::logging_get_logger("render");
}

void Render::render(core::Level* plevel)
{
    int camera_offset_x, camera_offset_y;
    float camera_zoom;
    get_camera_offsets(&camera_offset_x, &camera_offset_y, &camera_zoom);
    m_p_renderer->clear();
    render_level(plevel, camera_offset_x, camera_offset_y);
    render_entities(camera_offset_x, camera_offset_y);
    m_p_renderer->present();
}

void Render::get_camera_offsets(
    int* p_camera_offset_x, int* p_camera_offset_y, float* p_camera_zoom)
{
    // TODO(Keegan): Verify only one camera entity, error otherwise
    auto cameraentities = m_camerasystem.getEntities();
    auto& cameraentity = cameraentities.front();
    auto& camera_camera_component =
        cameraentity.getComponent<components::CameraComponent>();
    auto& camera_transform_component =
        cameraentity.getComponent<components::TransformComponent>();

    int screen_w, screen_h;
    m_p_window->get_size(&screen_w, &screen_h);
    *p_camera_offset_x = camera_transform_component.pos_x - screen_w / 2;
    *p_camera_offset_y = camera_transform_component.pos_y - screen_h / 2;
    *p_camera_zoom = camera_camera_component.zoom;
}

void Render::render_entities(
    int camera_offset_x, int camera_offset_y, float camera_zoom)
{
    auto entities = getEntities();

    for (auto& entity : entities)
    {
        auto& texture_component =
            entity.getComponent<components::TextureComponent>();
        auto& transform_component =
            entity.getComponent<components::TransformComponent>();
        SDL_Rect dest_rect = {
            static_cast<int>(transform_component.pos_x - camera_offset_x),
            static_cast<int>(transform_component.pos_y - camera_offset_y),
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

void Render::render_level(core::Level* plevel,
    int camera_offset_x,
    int camera_offset_y,
    float camera_zoom)
{
    uint16_t size_x, size_y;
    plevel->get_size(size_x, size_y);
    auto scale = plevel->get_scale();

    // TODO: Add some smartness to clip out irrelevant portions of the level
    for (uint16_t pos_y = 0; pos_y < size_y; pos_y++)
    {
        for (uint16_t pos_x = 0; pos_x < size_x; pos_x++)
        {
            SDL_Rect dest_rect = {
                static_cast<int>((pos_x * scale) - camera_offset_x),
                static_cast<int>((pos_y * scale) - camera_offset_y),
                static_cast<int>((pos_x + 1) * scale),
                static_cast<int>((pos_y + 1) * scale)};
            uint8_t red, blue, green;

            auto ptile = plevel->get(pos_x, pos_y);
            if (ptile != nullptr)
            {
                ptile->get_color(red, blue, green);
                m_p_renderer->set_draw_color(red, blue, green, 255);
                m_p_renderer->fill_rect(&dest_rect);
            }
            else
            {
                m_sp_logger->error("Tile {},{} is nullptr", pos_x, pos_y);
            }
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
