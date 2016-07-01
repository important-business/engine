#include <iostream>
#include <sstream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <anax/anax.hpp>

#include <sdl/wrap.hpp>
#include "core/example_module.hpp"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "Engine";

bool handle_input();


int main(int argc, char* argv[])
{
    std::cout << "Yay, you built code!\n";
    auto a = core::ExampleModule(4);
    auto i = a.getme();
    std::cout << "Returned value " << i << "\n";

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    auto window = sdl::Window(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    auto renderer = sdl::Renderer(&window, -1, SDL_RENDERER_SOFTWARE);
    auto goose_texture = sdl::Texture("resources/angry_goose_head.png", &renderer);

    auto is_not_done = true;
    while (is_not_done)
    {
        is_not_done = handle_input();
        renderer.clear();
        renderer.copy(&goose_texture, nullptr, nullptr);
        renderer.present();
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
