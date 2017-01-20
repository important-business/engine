#include "components/collision.hpp"
#include "core/resource_manager.hpp"
#include "systems/render.hpp"

#include <cassert>

namespace systems
{

Render::Render(sdl_wrap::Window* p_window,
    const core::Configuration& config,
    Camera& camerasystem,
    core::ResourceManagerTexture* p_resourcemanager)
    : m_up_window(p_window),
      m_sp_logger(common::logging_get_logger("render")),
      m_camerasystem(camerasystem),
      m_up_resourcemanager(p_resourcemanager),
      m_render_collision(false)
{

    uint32_t render_flags = 0;
    // TODO(Keegan, Decide if need render to texture)
    if (config.get_render_acceleration())
    {
        render_flags |= SDL_RENDERER_ACCELERATED;
        m_sp_logger->info("Renderer using hardware acceleration");
    }
    else
    {
        render_flags |= SDL_RENDERER_SOFTWARE;
        m_sp_logger->info("Renderer using software");
    }
    if (config.get_render_vsync())
    {
        m_sp_logger->info("Renderer using vsync");
        render_flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    if (config.get_render_show_collision())
    {
        m_sp_logger->info("Renderer rendering collision");
        m_render_collision = true;
    }
    m_up_renderer =
        std::make_unique<sdl_wrap::Renderer>(*m_up_window, -1, render_flags);
    m_up_renderer->set_draw_color(150, 150, 150, 100);
    m_up_renderer->set_draw_blend_mode(SDL_BLENDMODE_BLEND);
}

void Render::render(core::Level* plevel)
{
    float camera_min_x, camera_min_y, camera_max_x, camera_max_y, camera_zoom;
    get_camera_extents(
        camera_min_x, camera_min_y, camera_max_x, camera_max_y, camera_zoom);
    /* m_sp_logger->info("Camera Extents: min_x{} min_y{} max_x{} max_y{}", */
    /*     camera_min_x, */
    /*     camera_min_y, */
    /*     camera_max_x, */
    /*     camera_max_y); */
    m_up_renderer->clear();
    render_level(plevel,
        camera_min_x,
        camera_min_y,
        camera_max_x,
        camera_max_y,
        camera_zoom);
    render_entities(
        camera_min_x, camera_min_y, camera_max_x, camera_max_y, camera_zoom);
    m_up_renderer->present();
}

void Render::get_camera_extents(float& camera_min_x,
    float& camera_min_y,
    float& camera_max_x,
    float& camera_max_y,
    float& camera_zoom)
{
    auto cameraentities = m_camerasystem.getEntities();
    auto& cameraentity = cameraentities.front();

    // Only allow one camera
    assert(cameraentities.size() == 1);

    // No hasComponent check is needed since camera system does filtering
    auto& camera_camera_component =
        cameraentity.getComponent<components::CameraComponent>();
    auto& camera_transform_component =
        cameraentity.getComponent<components::TransformComponent>();

    int screen_w, screen_h;
    m_up_window->get_size(&screen_w, &screen_h);
    camera_zoom = camera_camera_component.zoom;
    camera_min_x =
        camera_transform_component.pos_x - screen_w / (2.0f * camera_zoom);
    camera_max_x =
        camera_transform_component.pos_x + screen_w / (2.0f * camera_zoom);
    camera_min_y =
        camera_transform_component.pos_y - screen_h / (2.0f * camera_zoom);
    camera_max_y =
        camera_transform_component.pos_y + screen_h / (2.0f * camera_zoom);
}

void Render::render_entities(float camera_min_x,
    float camera_min_y,
    float camera_max_x,
    float camera_max_y,
    float camera_zoom)
{
    auto entities = getEntities();

    for (auto& entity : entities)
    {
        // No hasComponent check is needed since render system does filtering
        auto& texture_component =
            entity.getComponent<components::TextureComponent>();
        auto& transform_component =
            entity.getComponent<components::TransformComponent>();

        float x_center =
            camera_zoom * (-camera_min_x + transform_component.pos_x);
        float y_center =
            camera_zoom * (-camera_min_y + transform_component.pos_y);
        float w = camera_zoom * transform_component.size_x;
        float h = camera_zoom * transform_component.size_y;

        /* m_sp_logger->info("Entity transform results: world: X{} Y{} W{} H{}",
         */
        /*     transform_component.pos_x, */
        /*     transform_component.pos_y, */
        /*     transform_component.size_x, */
        /*     transform_component.size_y); */
        /* m_sp_logger->info("Entity transform results: camera: X{} Y{} W{}
         * H{}", */
        /*     x_center, */
        /*     y_center, */
        /*     w, */
        /*     h); */

        SDL_Rect dest_rect = {
            static_cast<int>(x_center - (w / 2.0f)),
            static_cast<int>(y_center - (h / 2.0f)),
            static_cast<int>(std::round(w)),
            static_cast<int>(std::round(h)),
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

        if (!texture_component.sp_texture)
        {
            texture_component.sp_texture =
                m_up_resourcemanager->get(texture_component.texture_path);
        }
        m_up_renderer->copy(*texture_component.sp_texture,
            nullptr,
            &dest_rect,
            static_cast<int>(transform_component.rotation),
            flip);

        if (m_render_collision && entity.hasComponent<components::Collision>())
        {
            auto& collision = entity.getComponent<components::Collision>();
            // TODO(Keegan, Use bounding box position as well)
            SDL_Rect dest_rect = {
                static_cast<int>(
                    x_center - (camera_zoom * collision.bounding_box.w / 2.0f)),
                static_cast<int>(
                    y_center - (camera_zoom * collision.bounding_box.h / 2.0f)),
                static_cast<int>(
                    std::round(collision.bounding_box.w * camera_zoom)),
                static_cast<int>(
                    std::round(collision.bounding_box.h * camera_zoom)),
            };
            m_up_renderer->set_draw_color(0, 255, 0, 128);
            m_up_renderer->fill_rect(&dest_rect);
        }
    }
}

void Render::render_level(core::Level* plevel,
    float camera_min_x,
    float camera_min_y,
    float camera_max_x,
    float camera_max_y,
    float camera_zoom)
{
    int size_x, size_y, size_layers;
    int level_min_x, level_min_y, level_max_x, level_max_y;

    plevel->get_size(size_x, size_y, size_layers);
    plevel->get_tile_coords(
        camera_min_x, camera_min_y, level_min_x, level_min_y);
    plevel->get_tile_coords(
        camera_max_x, camera_max_y, level_max_x, level_max_y);
    level_min_x -= 1;
    level_min_y -= 1;
    level_max_x += 1;
    level_max_y += 1;

    auto scale = plevel->get_scale();

    for (int pos_y = level_min_y; pos_y < level_max_y; pos_y++)
    {
        for (int pos_x = level_min_x; pos_x < level_max_x; pos_x++)
        {
            for (int layer = 0; layer < size_layers; layer++)
            {
                if (!m_render_collision &&
                    layer == plevel->get_collision_layer())
                {
                    break;
                }
                SDL_Rect dest_rect = {
                    static_cast<int>(
                        (pos_x * scale - camera_min_x) * camera_zoom),
                    static_cast<int>(
                        (pos_y * scale - camera_min_y) * camera_zoom),
                    static_cast<int>((1.0f) * scale * camera_zoom),
                    static_cast<int>((1.0f) * scale * camera_zoom)};

                auto tile = plevel->get_tile(pos_x, pos_y, layer);
                if (tile == 0)
                {
                    // Do not draw
                }
                else if (tile != -1)
                {
                    core::LevelTileSet* p_tileset = nullptr;
                    int tileset_tile = -1;
                    plevel->get_tileset(tile, p_tileset, tileset_tile);
                    assert(p_tileset != nullptr);
                    if (p_tileset == nullptr)
                    {
                        continue;
                    }
                    auto p_texture = p_tileset->get_texture();
                    if (p_texture == nullptr)
                    {
                        auto sp_texture = m_up_resourcemanager->get(
                            p_tileset->get_filename());
                        p_texture = sp_texture.get();
                        p_tileset->set_texture(sp_texture);
                    }
                    SDL_Rect src_rect;
                    p_tileset->getTileClipping(tileset_tile,
                        src_rect.x,
                        src_rect.y,
                        src_rect.w,
                        src_rect.h);
                    m_up_renderer->copy(
                        *p_texture, &src_rect, &dest_rect, 0, SDL_FLIP_NONE);
                }
                else
                {
                    /* m_sp_logger->error("Tile {},{} is invalid", pos_x,
                     * pos_y); */
                }
            }
        }
    }
}

sdl_wrap::Renderer* Render::get_renderer()
{
    return m_up_renderer.get();
}

sdl_wrap::Window* Render::get_window()
{
    return m_up_window.get();
}

} // namespace systems
