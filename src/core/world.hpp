#ifndef CORE_WORLD_HPP
#define CORE_WORLD_HPP

#include "core/level.hpp"
#include "core/resource_manager.hpp"
#include "systems/movement.hpp"
#include "systems/player_input.hpp"
#include "systems/render.hpp"
#include "systems/collision.hpp"

#include <anax/anax.hpp>
#include <memory>
#include <sdl_wrap/wrap.hpp>

namespace core
{

class World : public systems::Collision::Listener

{
public:
    World(sdl_wrap::Window* pwindow) : m_p_window(pwindow)
    {
    }

    void init(Uint32 sdl_render_flags);

    void deinit();

    void execute(float dt);

    void handle_input();

private:
    sdl_wrap::Window* m_p_window;

    std::unique_ptr<anax::World> m_up_anax_world;

    std::unique_ptr<systems::Camera> m_up_camera_system;

    std::unique_ptr<systems::Render> m_up_render_system;

    std::unique_ptr<systems::Movement> m_up_movement_system;

    std::unique_ptr<systems::PlayerInput> m_up_player_input_system;

    std::unique_ptr<systems::Collision> m_up_collision_system;

    std::unique_ptr<core::ResourceManagerTexture> m_up_texture_manager;

    std::unique_ptr<core::Level> m_up_level;

    anax::Entity m_player;

    void on_collision_occured(
        anax::Entity& e1, anax::Entity& e2, double delta_time);
};
}

#endif /* CORE_WORLD_HPP */
