#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#include "core/application.hpp"

int main(int argc, char* argv[])
{
    int to_return = 0;
    try
    {
        core::Application app{argc, argv};
        app.init();
        to_return = app.loop();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exiting with exception:" << std::endl << "\t";
        std::cerr << e.what() << std::endl;
        to_return = 1;
    }
    return to_return;
}
