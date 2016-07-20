#include "application.hpp"

using namespace core;

const int WINDOW_WIDTH{640};
const int WINDOW_HEIGHT{480};
const std::string WINDOW_TITLE{"Engine"};
const float MS_TO_SECONDS{1000.0};

void Application::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    pwindow = std::make_unique<sdl::Window>(WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    pworld = std::make_unique<core::World>(pwindow.get(), this);
    pworld->init(SDL_RENDERER_SOFTWARE);
}

int Application::loop()
{
    float time = (float)SDL_GetTicks();
    float lasttime = time;
    while (not toquit)
    {
        // TODO: Make this loop not suck as hard
        float dt = (time - lasttime) / MS_TO_SECONDS;
        lasttime = time;
        pworld->execute(dt);
        time = SDL_GetTicks();
    }
    pworld->deinit();
    return 0;
}

void Application::quit()
{
    toquit = true;
}
