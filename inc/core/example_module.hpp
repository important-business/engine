#ifndef __EXAMPLE_MODULE_HPP
#define __EXAMPLE_MODULE_HPP

namespace core{

class ExampleModule
{
public:
    ExampleModule(int initval = 0):value(initval){}
    void setme(int newvalue);
    int getme();
private:
    int value;
};

}
#endif //ndef __EXAMPLE_MODULE_HPP
