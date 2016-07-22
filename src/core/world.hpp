#ifndef CORE_WORLD_HPP
#define CORE_WORLD_HPP

#include <memory>
#include <anax/anax.hpp>
#include <sdl/wrap.hpp>
#include "systems/rendersystem.hpp"
#include "systems/movementsystem.hpp"
#include "systems/playerinputsystem.hpp"
#include "core/resourcemanager.hpp"
#include "core/application.hpp"

namespace core
{

class Application;

class World
{
public:
    World(sdl::Window* pwindow, core::Application* p_application)
        : m_p_window(pwindow), m_p_application(p_application)
    {
    }

    void init(Uint32 sdl_render_flags);

    void deinit();

    void execute(float dt);

    void handle_input();

private:
    sdl::Window* m_p_window;

    core::Application* m_p_application;

    std::unique_ptr<anax::World> m_up_anax_world;

    std::unique_ptr<systems::RenderSystem> m_up_render_system;

    std::unique_ptr<systems::MovementSystem> m_up_movement_system;

    std::unique_ptr<systems::PlayerInputSystem> m_up_player_input_system;

    std::unique_ptr<core::ResourceManagerTexture> m_up_texture_manager;
};
}

#endif /* CORE_WORLD_HPP */
