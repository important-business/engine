#include "systems/ai.hpp"
#include "components/velocity.hpp"
#include "components/transform.hpp"

#include <iostream>
#include <algorithm>

namespace systems
{

AiResult AiNode::execute(anax::Entity entity)
{
    return _execute(entity);
}

void AiNode::success(anax::Entity entity)
{
    return _success(entity);
}

void AiNode::failure(anax::Entity entity)
{
    return _failure(entity);
}

AiResult AiNode::_execute(anax::Entity entity)
{
    return AI_RESULT_SUCCESS;
}

void AiNode::_success(anax::Entity entity)
{
}

void AiNode::_failure(anax::Entity entity)
{
}

void AiNodeComposite::add_child(AiNode* p_ai_node)
{
    m_v_up_children.push_back(std::unique_ptr<AiNode>(p_ai_node));
}

AiResult AiNodeSequence::_execute(anax::Entity entity)
{
    AiResult result;
    if (m_v_up_children.size() == 0)
    {
        result = AI_RESULT_SUCCESS;
    }
    else
    {
        auto p_firstnode = m_v_up_children.front().get();
        auto status = p_firstnode->execute(entity);
        switch (status)
        {
        case AI_RESULT_FAIL:
            p_firstnode->failure(entity);
            break;
        case AI_RESULT_SUCCESS:
            std::cout << "next in sequence!" << std::endl;
            p_firstnode->success(entity);
            m_v_up_children.erase(m_v_up_children.begin());
            if (m_v_up_children.size() == 0)
            {
                result = status;
            }
            break;
        case AI_RESULT_READY:
        default:
            result = status;
        }
    }
    return result;
}

AiResult AiNodeLoop::_execute(anax::Entity entity)
{
    AiResult result;
    if (m_v_up_children.size() == 0)
    {
        result = AI_RESULT_SUCCESS;
    }
    else
    {
        auto p_firstnode = m_v_up_children.front().get();
        auto status = p_firstnode->execute(entity);
        switch (status)
        {
        case AI_RESULT_FAIL:
            p_firstnode->failure(entity);
            break;
        case AI_RESULT_SUCCESS:
            std::cout << "next in sequence!" << std::endl;
            p_firstnode->success(entity);
            /* m_v_up_children.erase(m_v_up_children.begin()); */
            std::rotate(m_v_up_children.begin(),
                m_v_up_children.begin() + 1,
                m_v_up_children.end());
            if (m_v_up_children.size() == 0)
            {
                result = status;
            }
            break;
        case AI_RESULT_READY:
        default:
            result = status;
        }
    }
    return result;
}

AiResult AiNodeSelector::_execute(anax::Entity entity)
{
    AiResult result;
    if (m_v_up_children.size() == 0)
    {
        result = AI_RESULT_FAIL;
    }
    else
    {
        auto p_firstnode = m_v_up_children.front().get();
        auto status = p_firstnode->execute(entity);
        switch (status)
        {
        case AI_RESULT_FAIL:
            p_firstnode->failure(entity);
            m_v_up_children.erase(m_v_up_children.begin());
            if (m_v_up_children.size() == 0)
            {
                result = status;
            }
            break;
        case AI_RESULT_SUCCESS:
            p_firstnode->success(entity);
            result = status;
            break;
        case AI_RESULT_READY:
        default:
            result = status;
        }
    }
    return result;
}

AiResult AiNodeDecorator::_execute(anax::Entity entity)
{
    return m_up_decoratee->execute(entity);
}
void AiNodeDecorator::_success(anax::Entity entity)
{
    return m_up_decoratee->success(entity);
}
void AiNodeDecorator::_failure(anax::Entity entity)
{
    return m_up_decoratee->failure(entity);
}

AiResult AiNodeDecoratorInvert::_execute(anax::Entity entity)
{
    auto status = m_up_decoratee->execute(entity);
    AiResult result;
    switch (status)
    {
    case AI_RESULT_FAIL:
        result = AI_RESULT_SUCCESS;
        break;
    case AI_RESULT_SUCCESS:
        result = AI_RESULT_FAIL;
        break;
    case AI_RESULT_READY:
    default:
        result = status;
    }
    return result;
}

AiNodeMoveTo::AiNodeMoveTo(double pos_x, double pos_y, double tolerance)
    : m_pos_x(pos_x), m_pos_y(pos_y), m_tolerance(tolerance)
{
}

AiResult AiNodeMoveTo::_execute(anax::Entity entity)
{
    auto& transform_component =
        entity.getComponent<components::TransformComponent>();
    auto& velocity_component =
        entity.getComponent<components::VelocityComponent>();
    const float move_acel = 150;
    const float top_speed = 500;
    double delta_x = m_pos_x - transform_component.pos_x;
    double delta_y = m_pos_y - transform_component.pos_y;
    auto result = AI_RESULT_SUCCESS;
    if (delta_x > 0 + m_tolerance)
    {
        if (velocity_component.velocity.x < top_speed)
        {
            velocity_component.force.x += move_acel;
        }
        result = AI_RESULT_READY;
    }
    else if (delta_x < 0 - m_tolerance)
    {
        if (velocity_component.velocity.x > -top_speed)
        {
            velocity_component.force.x -= move_acel;
        }
        result = AI_RESULT_READY;
    }
    else
    {
        velocity_component.force.x = -velocity_component.velocity.x;
    }
    if (delta_y > 0 + m_tolerance)
    {
        if (velocity_component.velocity.y < top_speed)
        {
            velocity_component.force.y += move_acel;
        }
        result = AI_RESULT_READY;
    }
    else if (delta_y < 0 - m_tolerance)
    {
        if (velocity_component.velocity.y > -top_speed)
        {
            velocity_component.force.y -= move_acel;
        }
        result = AI_RESULT_READY;
    }
    else
    {
        velocity_component.force.y = -velocity_component.velocity.y;
    }
    return result;
}

AiSystem::AiSystem()
{
    m_sp_logger = core::logging_get_logger("ai");
}

void AiSystem::update()
{
    auto entities = getEntities();
    for (auto& entity : entities)
    {
        auto& ai_component = entity.getComponent<components::AiComponent>();
        if (ai_component.up_root_node)
        {
            auto result = ai_component.up_root_node->execute(entity);

            switch (result)
            {
            case AI_RESULT_FAIL:
                m_sp_logger->info("AI processing failure");
                ai_component.up_root_node.reset();
                break;
            case AI_RESULT_SUCCESS:
                m_sp_logger->info("AI processing success");
                ai_component.up_root_node.reset();
                break;
            case AI_RESULT_READY:
                break;
            default:
                m_sp_logger->error("Invalid AI state");
                break;
            }
        }
    }
}

} // namespace components
