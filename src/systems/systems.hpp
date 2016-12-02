#ifndef SYSTEMS_SYSTEMS_HPP
#define SYSTEMS_SYSTEMS_HPP

#include "core/exception.hpp"
#include "components/id.hpp"
#include <anax/Entity.hpp>

namespace systems
{
template <class T>
class ExceptionMissingComponent : public core::Exception
{
public:
    ExceptionMissingComponent(anax::Entity e)
    : Exception("Missing component '")
    {
        m_message.append(T::name);
        m_message.append("' on entity id '");
        // All correctly constructed entities will have an ID component
        m_message.append(e.getComponent<components::IdComponent>().id);
        m_message.append("'.");
    }
};

template<class T>
void inline throw_if_missing_component(anax::Entity entity){
    if (!entity.hasComponent<T>())
    {
        throw ExceptionMissingComponent<T>(entity);
    }
}


} // namespace systems
#endif /* SYSTEMS_SYSTEMS_HPP */
