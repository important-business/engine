#ifndef COMPONENTS_AI_HPP
#define COMPONENTS_AI_HPP

#include <anax/Component.hpp>
#include <anax/Entity.hpp>
#include <memory>
#include <string>
#include <vector>

namespace systems
{
class AiNode;
} // namespace components

namespace components
{

extern const std::string str_name_ai;

struct AiComponent : anax::Component
{
    AiComponent(systems::AiNode* p_root_node);

    std::unique_ptr<systems::AiNode> up_root_node;
    float move_accel;
    float top_speed;
};

} // namespace components
#endif /* COMPONENTS_AI_HPP */
