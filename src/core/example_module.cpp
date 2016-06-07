#include "core/example_module.hpp"

using namespace core;

int ExampleModule::getme(void){
    return value;
}

void ExampleModule::setme(int newvalue){
    value = newvalue;
}

