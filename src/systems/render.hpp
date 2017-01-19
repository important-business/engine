#ifndef SYSTEMS_RENDER_HPP
#define SYSTEMS_RENDER_HPP

#include "systems.hpp"

#include "components/camera.hpp"
#include "components/collision.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "core/configuration.hpp"
#include "core/level.hpp"
#include "core/logging.hpp"

#include <string>
#include <anax/System.hpp>

namespace core
{
class ResourceManagerTexture;
}

namespace systems
{

struct Camera : anax::System<anax::Requires<components::CameraComponent,
                    components::TransformComponent>>
{
    Camera() = default;

    ~Camera() = default;

private:
};

struct Render : anax::System<anax::Requires<components::TextureComponent,
                    components::TransformComponent>>
{
    Render(sdl_wrap::Window* p_window,
        const core::Configuration& config,
        Camera& camerasystem,
        core::ResourceManagerTexture* p_resourcemanager);

    ~Render() = default;

    void render(core::Level* plevel);

    bool is_valid() const;

    sdl_wrap::Renderer* get_renderer();

    sdl_wrap::Window* get_window();

private:
    void get_camera_extents(
        float& camera_min_x, float& camera_min_y, float &camera_max_x,float &camera_max_y, float &camera_zoom);

    void render_entities(float camera_min_x = 0.0f,
        float camera_min_y = 0.0f,
        float camera_max_x = 0.0f,
        float camera_max_y = 0.0f,
        float camera_zoom = 1.0f);

    void render_level(core::Level* plevel,
        float camera_min_x = 0.0f,
        float camera_min_y = 0.0f,
        float camera_max_x = 0.0f,
        float camera_max_y = 0.0f,
        float camera_zoom = 1.0f);

    std::unique_ptr<sdl_wrap::Window> m_up_window;

    std::unique_ptr<sdl_wrap::Renderer> m_up_renderer;

    std::shared_ptr<spdlog::logger> m_sp_logger;

    Camera& m_camerasystem;

    std::unique_ptr<core::ResourceManagerTexture> m_up_resourcemanager;

    bool m_render_collision;
};

}
#endif /* SYSTEMS_RENDER_HPP */
