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
const int MS_TO_SECONDS{1000};

bool handle_input();

int main(int argc, char* argv[])
{
    try
    {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);

        auto pwindow = new sdl::Window{
            WINDOW_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN,
        };
        auto pworld = new core::World{pwindow};
        pworld->init(SDL_RENDERER_SOFTWARE);
        float time = (float)SDL_GetTicks();
        float lasttime = time;
        while (not pworld->isToQuit())
        {
            // TODO: Make this loop not suck as hard
            float dt = (time - lasttime) / MS_TO_SECONDS;
            lasttime = time;
            pworld->execute(dt);
            time = SDL_GetTicks();
        }
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
