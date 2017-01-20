#ifndef CORE_APPLICATION_HPP
#define CORE_APPLICATION_HPP

#include "core/world.hpp"

#include <sdl_wrap/wrap.hpp>

#include <memory>
#include <SDL.h>

namespace core
{

class World;

class Application
{
public:
    Application(int argc, char** argv, Configuration &config);

    void init();

    void deinit();

    int loop();

    void quit();

private:
    std::unique_ptr<core::World> m_up_world;

    std::unique_ptr<sdl_wrap::Window> m_up_window;

    bool m_has_quit = true;

    int argc;

    char** argv;

    Configuration &m_config;

    void handle_events();
};
}

#endif /* CORE_APPLICATION_HPP */
