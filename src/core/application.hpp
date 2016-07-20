#ifndef CORE_APPLICATION_HPP
#define CORE_APPLICATION_HPP

#include <memory>
#include <SDL.h>
#include "core/world.hpp"

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
    std::unique_ptr<core::World> pworld;
    std::unique_ptr<sdl::Window> pwindow;

    bool toquit;
    int argc;
    char** argv;
};
}

#endif /* CORE_APPLICATION_HPP */
