#include "core/example_module.hpp"

using namespace core;

int ExampleModule::get_me(void)
{
    return m_value;
}

void ExampleModule::set_me(int new_value)
{
    m_value = new_value;
}
