#ifndef CORE_WORLD_HPP
#define CORE_WORLD_HPP

#include <memory>
#include <anax/anax.hpp>
#include <sdl/wrap.hpp>
#include "systems/rendersystem.hpp"
#include "systems/movementsystem.hpp"
#include "systems/playerinputsystem.hpp"
#include "core/resourcemanager.hpp"

namespace core
{

class World
{
public:
    World(sdl::Window* pwindow) : pwindow(pwindow)
    {
    }
    void init(Uint32 sdlrenderflags);
    void deinit();
    void execute(float dt);
    void handle_input();
    bool isToQuit();

private:
    sdl::Window* pwindow;
    std::unique_ptr<anax::World> panaxworld;

    std::unique_ptr<systems::RenderSystem> prendersystem;

    std::unique_ptr<systems::MovementSystem> pmovementsystem;

    std::unique_ptr<systems::PlayerInputSystem> pplayerinputsystem;

    std::unique_ptr<core::ResourceManagerTexture> ptexturemanager;
    bool toquit;
};
}

#endif /* CORE_WORLD_HPP */
