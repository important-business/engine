#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#include <SDL.h>
#include <SDL_image.h>

#include <anax/anax.hpp>

#include <sdl/wrap.hpp>
#include "core/world.hpp"

const int WINDOW_WIDTH{640};
const int WINDOW_HEIGHT{480};
const std::string WINDOW_TITLE{"Engine"};
const float MS_TO_SECONDS{1000.0};

int main(int argc, char* argv[])
{
    int toreturn = 0;
    try
    {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);

        sdl::Window window = {
            WINDOW_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN,
        };
        core::World world = {&window};
        world.init(SDL_RENDERER_SOFTWARE);
        float time = (float)SDL_GetTicks();
        float lasttime = time;
        while (not world.isToQuit())
        {
            // TODO: Make this loop not suck as hard
            float dt = (time - lasttime) / MS_TO_SECONDS;
            lasttime = time;
            world.execute(dt);
            time = SDL_GetTicks();
        }
        world.deinit();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exiting with exception:" << std::endl << "\t";
        std::cerr << e.what() << std::endl;
        toreturn = 1;
    }
    return toreturn;
}
