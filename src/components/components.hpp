#ifndef COMPONENTS_COMPONENTS_HPP
#define COMPONENTS_COMPONENTS_HPP

#include <anax/Component.hpp>
#include <string>

namespace components
{

struct Component : anax::Component
{
    static const std::string name;
};

} // namespace components
#endif /* COMPONENTS_COMPONENTS_HPP */
