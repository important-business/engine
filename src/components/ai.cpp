#include "ai.hpp"
#include "systems/ai.hpp"

namespace components
{
AiComponent::AiComponent(systems::AiNode* p_root_node)
    : up_root_node(p_root_node)
{
}

} // namespace components
