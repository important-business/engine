#include "application.hpp"
#include "configuration.hpp"

#include <fstream>
#include <json/json.h>

namespace core
{

const char* WINDOW_TITLE{"Engine"};
const char* CONFIG_PATH{"config.json"};
const float MS_TO_SECONDS{1000.0};
const float S_PER_UPDATE{0.016667};

void Application::init()
{
    sdl_wrap::sdl_init(SDL_INIT_EVERYTHING, IMG_INIT_PNG);

    Configuration config{CONFIG_PATH};

    auto window_width = config.get_window_width();
    auto window_height = config.get_window_height();

    m_up_window = std::make_unique<sdl_wrap::Window>(WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_SHOWN);
    m_up_world = std::make_unique<core::World>(m_up_window.get(), config);
    m_up_world->init();
    m_has_quit = false;
}

int Application::loop()
{
    double cur_time = SDL_GetTicks() / MS_TO_SECONDS;
    double last_time = cur_time;
    double lag = 0.0;
    while (not m_has_quit)
    {
        lag += cur_time - last_time;
        last_time = cur_time;
        while (lag >= S_PER_UPDATE)
        {
            m_up_world->execute_fixed(S_PER_UPDATE);
            lag -= S_PER_UPDATE;
        }
        cur_time = SDL_GetTicks() / MS_TO_SECONDS;
        handle_events();

        m_up_world->execute(cur_time - last_time);
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
