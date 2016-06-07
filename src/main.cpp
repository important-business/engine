#include <iostream>
#include "core/example_module.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Yay, you built code!\n";
    auto a = core::ExampleModule(4);
    auto i = a.getme();
    std::cout << "Returned value " << i << "\n";
}
