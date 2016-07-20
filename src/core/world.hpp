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
    World(sdl::Window* pwindow, core::Application* papplication)
        : pwindow(pwindow), papplication(papplication)
    {
    }
    void init(Uint32 sdlrenderflags);
    void deinit();
    void execute(float dt);
    void handle_input();

private:
    sdl::Window* pwindow;
    core::Application* papplication;

    std::unique_ptr<anax::World> panaxworld;

    std::unique_ptr<systems::RenderSystem> prendersystem;

    std::unique_ptr<systems::MovementSystem> pmovementsystem;

    std::unique_ptr<systems::PlayerInputSystem> pplayerinputsystem;

    std::unique_ptr<core::ResourceManagerTexture> ptexturemanager;
};
}

#endif /* CORE_WORLD_HPP */
