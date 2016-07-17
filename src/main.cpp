#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#include <SDL.h>
#include <SDL_image.h>

#include <anax/anax.hpp>

#include <sdl/wrap.hpp>
#include "systems/rendersystem.hpp"
#include "systems/movementsystem.hpp"
#include "systems/playerinputsystem.hpp"
#include "components/transform.hpp"
#include "components/render.hpp"
#include "components/velocity.hpp"
#include "components/player.hpp"
#include "core/resourcemanager.hpp"

const int WINDOW_WIDTH{640};
const int WINDOW_HEIGHT{480};
const std::string WINDOW_TITLE{"Engine"};

bool handle_input();

anax::Entity goose_factory(anax::World* pworld,
    core::ResourceManagerTexture* ptexturemanager,
    float posx,
    float posy)
{
    auto pgoose_texture =
        ptexturemanager->get(std::string("resources/angry_goose_head.png"));

    auto entity = pworld->createEntity();

    auto& sprite = entity.addComponent<components::TextureComponent>();
    sprite.ptexture = pgoose_texture;

    (void)entity.addComponent<components::TransformComponent>(
        posx, posy, 128.0f, 128.0f, 0.0f, false, true);

    (void)entity.addComponent<components::VelocityComponent>();

    (void)entity.addComponent<components::PlayerComponent>();

    entity.activate();

    return entity;
}

int main(int argc, char* argv[])
{
    try
    {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);

        systems::RenderSystem rendersystem{WINDOW_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN,
            SDL_RENDERER_SOFTWARE};

        systems::MovementSystem movementsystem;
        systems::PlayerInputSystem playerinputsystem;

        anax::World world{};
        core::ResourceManagerTexture texturemanager{};

        texturemanager.setDefaultRenderer(rendersystem.getRenderer());

        auto goose1 = goose_factory(&world, &texturemanager, 100.0, 300.0);

        auto goose2 = goose_factory(&world, &texturemanager, 100.0, 200.0);

        auto goose3 = goose_factory(&world, &texturemanager, 100.0, 100.0);

        world.addSystem(rendersystem);
        world.addSystem(movementsystem);
        world.addSystem(playerinputsystem);

        auto is_not_done = true;
        auto tick = 1;
        while (is_not_done)
        {
            world.refresh();
            rendersystem.render();
            movementsystem.update(tick);
            playerinputsystem.update(tick);
            is_not_done = handle_input();
        }
        goose1.kill();
        goose2.kill();
        // Refresh call needed for entity and components to be deleted
        world.refresh();
        texturemanager.unloadUnused();
        goose3.kill();
        world.refresh();
        texturemanager.unloadUnused();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exiting with exception:" << std::endl << "\t";
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

bool handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
        else
        {
        }
    }
    return true;
}
