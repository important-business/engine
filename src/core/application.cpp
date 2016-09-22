#include "application.hpp"
#include <iostream>

namespace core
{

const int WINDOW_WIDTH{640};
const int WINDOW_HEIGHT{480};
const char* WINDOW_TITLE{"Engine"};
const float MS_TO_SECONDS{1000.0};

void Application::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    m_up_window = std::make_unique<sdl_wrap::Window>(WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    m_up_world = std::make_unique<core::World>(m_up_window.get());
    m_up_world->init(SDL_RENDERER_SOFTWARE);
    m_has_quit = false;
}

int Application::loop()
{
    float time = SDL_GetTicks();
    auto last_time = time;
    while (not m_has_quit)
    {
        // TODO(Keegan): Make this loop not suck as hard
        auto delta_time = (time - last_time) / MS_TO_SECONDS;
        last_time = time;
        m_up_world->execute(delta_time);
        time = SDL_GetTicks();
        handle_events();
    }
    m_up_world->deinit();
    return 0;
}

void Application::quit()
{
    m_has_quit = true;
}

void Application::handle_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            Application::quit();
        }
        else
        {
        }
    }
}

} // namespace core
