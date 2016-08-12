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
    Application(int argc, char** argv) : argc(argc), argv(argv)
    {
    }

    void init();

    void deinit();

    int loop();

    void handle_input();

    void quit();

private:
    std::unique_ptr<core::World> m_up_world;

    std::unique_ptr<sdl_wrap::Window> m_up_window;

    bool m_has_quit;

    int argc;

    char** argv;
};
}

#endif /* CORE_APPLICATION_HPP */
