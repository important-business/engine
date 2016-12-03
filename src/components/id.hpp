#ifndef COMPONENTS_ID_HPP
#define COMPONENTS_ID_HPP

#include <anax/Component.hpp>
#include <string>

namespace components
{

struct IdComponent : anax::Component
{
    IdComponent(std::string id) : id(id)
    {
    }

    const std::string id;

    static const std::string name;

};
}

#endif /* COMPONENTS_ID_HPP */