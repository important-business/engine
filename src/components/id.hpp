/**
 * @file
 * @brief ID Component Header
 *
 * Contains definition of ID component
 */

#ifndef COMPONENTS_ID_HPP
#define COMPONENTS_ID_HPP

#include <anax/Component.hpp>
#include <string>

namespace components
{

struct IdComponent : anax::Component
{
    explicit IdComponent(std::string id) : id(id)
    {
    }

    const std::string id;

    static const std::string name;

};
}

#endif /* COMPONENTS_ID_HPP */
