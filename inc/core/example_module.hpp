#ifndef EXAMPLE_MODULE_HPP
#define EXAMPLE_MODULE_HPP

namespace core
{

class ExampleModule
{
public:
    ExampleModule(int init_val = 0) : m_value(init_val)
    {
    }
    void set_me(int new_value);
    int get_me();

private:
    int m_value;
};
}
#endif /* EXAMPLE_MODULE_HPP */
