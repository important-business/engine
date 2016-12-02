#ifndef SYSTEMS_RENDER_HPP
#define SYSTEMS_RENDER_HPP

#include "systems.hpp"

#include "components/camera.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
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
        Uint32 render_flags,
        Camera& camerasystem,
        core::ResourceManagerTexture* p_resourcemanager);

    ~Render() = default;

    void render(core::Level* plevel);

    bool is_valid() const;

    sdl_wrap::Renderer* get_renderer() const;

    sdl_wrap::Window* get_window() const;

private:
    void get_camera_offsets(
        int* p_camera_offset_x, int* p_camera_offset_y, float* p_camera_zoom);

    void render_entities(int camera_offset_x = 0,
        int camera_offset_y = 0,
        float camera_zoom = 1.0);

    void render_level(core::Level* plevel,
        int camera_offset_x = 0,
        int camera_offset_y = 0,
        float camera_zoom = 1.0);

    sdl_wrap::Window* m_p_window;

    sdl_wrap::Renderer* m_p_renderer;

    std::shared_ptr<spdlog::logger> m_sp_logger;

    Camera& m_camerasystem;

    core::ResourceManagerTexture* m_p_resourcemanager;
};
}
#endif /* SYSTEMS_RENDER_HPP */
