#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#include "core/application.hpp"

int main(int argc, char* argv[])
{
    int toreturn = 0;
    try
    {
        core::Application app{argc, argv};
        app.init();
        toreturn = app.loop();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exiting with exception:" << std::endl << "\t";
        std::cerr << e.what() << std::endl;
        toreturn = 1;
    }
    return toreturn;
}
