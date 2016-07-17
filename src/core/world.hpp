#ifndef CORE_WORLD_HPP
#define CORE_WORLD_HPP

#include <anax/anax.hpp>
#include <sdl/wrap.hpp>
#include "systems/rendersystem.hpp"
#include "systems/movementsystem.hpp"
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
    anax::World* panaxworld;

    systems::RenderSystem* prendersystem;

    systems::MovementSystem* pmovementsystem;

    core::ResourceManagerTexture* ptexturemanager;
    bool toquit;
};
}

#endif /* CORE_WORLD_HPP */
