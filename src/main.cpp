#include <iostream>
#include <sstream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <anax/anax.hpp>

#include <sdl/wrap.hpp>
#include "systems/rendersystem.hpp"
#include "components/transform.hpp"
#include "components/render.hpp"
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

    auto player = pworld->createEntity();

    auto& sprite = player.addComponent<components::TextureComponent>();
    (void)player.addComponent<components::TransformComponent>(
        posx, posy, 128.0f, 128.0f, 0.0f, false, true);
    sprite.ptexture = pgoose_texture;

    player.activate();

    return player;
}

int main(int argc, char* argv[])
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

    anax::World world{};
    core::ResourceManagerTexture texturemanager{};

    texturemanager.setDefaultRenderer(rendersystem.getRenderer());

    (void)goose_factory(&world, &texturemanager, 100.0, 300.0);

    (void)goose_factory(&world, &texturemanager, 100.0, 200.0);

    (void)goose_factory(&world, &texturemanager, 100.0, 100.0);

    world.addSystem(rendersystem);
    auto is_not_done = true;
    while (is_not_done)
    {
        world.refresh();
        rendersystem.render();
        is_not_done = handle_input();
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
