#include "core/example_module.hpp"

namespace core
{

int ExampleModule::get_me()
{
    return m_value;
}

void ExampleModule::set_me(int new_value)
{
    m_value = new_value;
}

} // namespace core
