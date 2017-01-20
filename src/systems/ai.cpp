/**
 * @file
 * @brief System Ai Source
 *
 * Contains implementation of classes for Ai Logic
 */

#include "common/exception.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "systems/ai.hpp"

#include <iostream>
#include <algorithm>

namespace systems
{

AiResult AiNode::execute(anax::Entity entity, AiSystem const& aisystem)
{
    return _execute(entity, aisystem);
}

void AiNode::success(anax::Entity entity, AiSystem const& aisystem)
{
    return _success(entity, aisystem);
}

void AiNode::failure(anax::Entity entity, AiSystem const& aisystem)
{
    return _failure(entity, aisystem);
}

AiResult AiNode::_execute(anax::Entity entity, AiSystem const& aisystem)
{
    return AI_RESULT_SUCCESS;
}

void AiNode::_success(anax::Entity entity, AiSystem const& aisystem)
{
}

void AiNode::_failure(anax::Entity entity, AiSystem const& aisystem)
{
}

void AiNodeComposite::add_child(AiNode* p_ai_node)
{
    m_v_up_children.push_back(std::unique_ptr<AiNode>(p_ai_node));
}

int AiNodeComposite::get_child_count()
{
    return m_v_up_children.size();
}

AiResult AiNodeSequence::_execute(anax::Entity entity, AiSystem const& aisystem)
{
    AiResult result;
    if (m_v_up_children.empty())
    {
        result = AI_RESULT_SUCCESS;
    }
    else
    {
        auto p_firstnode = m_v_up_children.front().get();
        auto status = p_firstnode->execute(entity, aisystem);
        switch (status)
        {
        case AI_RESULT_FAIL:
            p_firstnode->failure(entity, aisystem);
            result = AI_RESULT_FAIL;
            break;
        case AI_RESULT_SUCCESS:
            p_firstnode->success(entity, aisystem);
            m_v_up_children.erase(m_v_up_children.begin());
            if (m_v_up_children.empty())
            {
                result = AI_RESULT_SUCCESS;
            }
            else
            {
                result = AI_RESULT_READY;
            }
            break;
        case AI_RESULT_READY:
        default:
            result = AI_RESULT_READY;
        }
    }
    return result;
}

AiResult AiNodeLoop::_execute(anax::Entity entity, AiSystem const& aisystem)
{
    AiResult result;
    if (m_v_up_children.empty())
    {
        result = AI_RESULT_SUCCESS;
    }
    else
    {
        auto p_firstnode = m_v_up_children.front().get();
        auto status = p_firstnode->execute(entity, aisystem);
        switch (status)
        {
        case AI_RESULT_FAIL:
            p_firstnode->failure(entity, aisystem);
            result = AI_RESULT_FAIL;
            break;
        case AI_RESULT_SUCCESS:
            p_firstnode->success(entity, aisystem);
            std::rotate(m_v_up_children.begin(),
                m_v_up_children.begin() + 1,
                m_v_up_children.end());
            if (m_v_up_children.empty())
            {
                result = AI_RESULT_SUCCESS;
            }
            else
            {
                result = AI_RESULT_READY;
            }
            break;
        case AI_RESULT_READY:
        default:
            result = AI_RESULT_READY;
            break;
        }
    }
    return result;
}

AiResult AiNodeSelector::_execute(anax::Entity entity, AiSystem const& aisystem)
{
    AiResult result;
    if (m_v_up_children.empty())
    {
        result = AI_RESULT_FAIL;
    }
    else
    {
        auto p_firstnode = m_v_up_children.front().get();
        auto status = p_firstnode->execute(entity, aisystem);
        switch (status)
        {
        case AI_RESULT_FAIL:
            p_firstnode->failure(entity, aisystem);
            m_v_up_children.erase(m_v_up_children.begin());
            if (m_v_up_children.empty())
            {
                result = AI_RESULT_FAIL;
            }
            else
            {
                result = AI_RESULT_READY;
            }
            break;
        case AI_RESULT_SUCCESS:
            p_firstnode->success(entity, aisystem);
            result = AI_RESULT_SUCCESS;
            break;
        case AI_RESULT_READY:
        default:
            result = AI_RESULT_READY;
            break;
        }
    }
    return result;
}

AiResult AiNodeDecorator::_execute(
    anax::Entity entity, AiSystem const& aisystem)
{
    AiResult result;
    if (m_up_decoratee != nullptr)
    {
        result = m_up_decoratee->execute(entity, aisystem);
    }
    else
    {
        result = AI_RESULT_FAIL;
    }
    return result;
}
void AiNodeDecorator::_success(anax::Entity entity, AiSystem const& aisystem)
{
    if (m_up_decoratee != nullptr)
    {
        return m_up_decoratee->success(entity, aisystem);
    }
}
void AiNodeDecorator::_failure(anax::Entity entity, AiSystem const& aisystem)
{
    if (m_up_decoratee != nullptr)
    {
        return m_up_decoratee->failure(entity, aisystem);
    }
}

AiResult AiNodeDecoratorInvert::_execute(
    anax::Entity entity, AiSystem const& aisystem)
{
    AiResult result = AI_RESULT_FAIL;
    if (m_up_decoratee != nullptr)
    {
        auto status = m_up_decoratee->execute(entity, aisystem);
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
            break;
        }
    }
    return result;
}

void AiNodeDecoratorInvert::_success(
    anax::Entity entity, AiSystem const& aisystem)
{
    if (m_up_decoratee != nullptr)
    {
        return m_up_decoratee->failure(entity, aisystem);
    }
}
void AiNodeDecoratorInvert::_failure(
    anax::Entity entity, AiSystem const& aisystem)
{
    if (m_up_decoratee != nullptr)
    {
        return m_up_decoratee->success(entity, aisystem);
    }
}

AiNodeMoveTo::AiNodeMoveTo(double pos_x, double pos_y, double tolerance)
    : m_pos_x(pos_x), m_pos_y(pos_y), m_tolerance(tolerance)
{
}

AiResult AiNodeMoveTo::_execute(anax::Entity entity, AiSystem const& aisystem)
{
    throw_if_missing_component<components::TransformComponent>(entity);
    auto& transform_component =
        entity.getComponent<components::TransformComponent>();

    double delta_x = m_pos_x - transform_component.pos_x;
    double delta_y = m_pos_y - transform_component.pos_y;
    auto result = AI_RESULT_SUCCESS;
    float vel_x = 0.0f, vel_y = 0.0f;
    if (delta_x > 0 + m_tolerance)
    {
        vel_x = 1.0;
        result = AI_RESULT_READY;
    }
    else if (delta_x < 0 - m_tolerance)
    {
        vel_x = -1.0;
        result = AI_RESULT_READY;
    }
    if (delta_y > 0 + m_tolerance)
    {
        vel_y = 1.0;
        result = AI_RESULT_READY;
    }
    else if (delta_y < 0 - m_tolerance)
    {
        vel_y = -1.0;
        result = AI_RESULT_READY;
    }
    aisystem.m_movement_signal.emit(entity, vel_x, vel_y);
    return result;
}

AiNodeFollow::AiNodeFollow(
    anax::Entity target, double tolerance, bool follow_x, bool follow_y)
    : m_target(target),
      m_tolerance(tolerance),
      m_follow_x(follow_x),
      m_follow_y(follow_y)
{
}

AiResult AiNodeFollow::_execute(anax::Entity entity, AiSystem const& aisystem)
{

    throw_if_missing_component<components::TransformComponent>(entity);
    auto& transform_component =
        entity.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::TransformComponent>(m_target);
    auto& target_transform_component =
        m_target.getComponent<components::TransformComponent>();

    double delta_x =
        target_transform_component.pos_x - transform_component.pos_x;
    double delta_y =
        target_transform_component.pos_y - transform_component.pos_y;
    float vel_x = 0.0f, vel_y = 0.0f;
    auto result = AI_RESULT_SUCCESS;
    if (m_follow_x)
    {
        if (delta_x > 0 + m_tolerance)
        {
            vel_x = 1.0;
            result = AI_RESULT_READY;
        }
        else if (delta_x < 0 - m_tolerance)
        {
            vel_x = -1.0;
            result = AI_RESULT_READY;
        }
    }
    if (m_follow_y)
    {
        if (delta_y > 0 + m_tolerance)
        {
            vel_y = 1.0;
            result = AI_RESULT_READY;
        }
        else if (delta_y < 0 - m_tolerance)
        {
            vel_y = -1.0;
            result = AI_RESULT_READY;
        }
    }
    aisystem.m_movement_signal.emit(entity, vel_x, vel_y);
    return result;
}
AiSystem::AiSystem() : m_sp_logger(common::logging_get_logger("ai"))
{
}

void AiSystem::update()
{
    auto entities = getEntities();
    for (auto& entity : entities)
    {
        auto& ai_component = entity.getComponent<components::AiComponent>();
        if (ai_component.up_root_node)
        {
            auto result = ai_component.up_root_node->execute(entity, *this);

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

} // namespace systems
