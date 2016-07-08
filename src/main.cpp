#include <iostream>
#include <sstream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <anax/anax.hpp>

#include <sdl/wrap.hpp>
#include "core/example_module.hpp"
#include "systems/rendersystem.hpp"
#include "components/transform.hpp"
#include "components/render.hpp"

const int WINDOW_WIDTH { 640 };
const int WINDOW_HEIGHT { 480 };
const std::string WINDOW_TITLE { "Engine" };

bool handle_input();


int main(int argc, char* argv[])
{
    std::cout << "Yay, you built code!\n";
    core::ExampleModule a { 4 };
    auto i = a.getme();
    std::cout << "Returned value " << i << "\n";

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    systems::RenderSystem rendersystem {
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN,
        SDL_RENDERER_SOFTWARE
    };

    sdl::Texture goose_texture {
            "resources/angry_goose_head.png",
            *rendersystem.getRenderer()
        };


    anax::World world{};
    auto player = world.createEntity();
    auto is_not_done = true;

    auto& sprite = player.addComponent<components::TextureComponent>();
    (void) player.addComponent<components::TransformComponent>(
        100.0f, 300.0f, 128.0f, 128.0f, 0.0f);
    sprite.ptexture = &goose_texture;

    player.activate();

    world.addSystem(rendersystem);
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
