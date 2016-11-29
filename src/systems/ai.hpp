#ifndef SYSTEMS_AI_HPP
#define SYSTEMS_AI_HPP
#include "components/ai.hpp"
#include "core/logging.hpp"

#include <anax/System.hpp>

namespace systems
{

enum AiResult
{
    AI_RESULT_READY,
    AI_RESULT_FAIL,
    AI_RESULT_SUCCESS,
    AI_RESULT_COUNT
};

class AiNode
{
public:
    AiResult execute(anax::Entity entity);
    void success(anax::Entity entity);
    void failure(anax::Entity entity);

private:
    virtual AiResult _execute(anax::Entity entity);
    virtual void _success(anax::Entity entity);
    virtual void _failure(anax::Entity entity);
};

class AiNodeComposite : public AiNode
{
public:
    void add_child(AiNode* p_ai_node);

protected:
    std::vector<std::unique_ptr<AiNode>> m_v_up_children;
};

class AiNodeSequence : public AiNodeComposite
{
public:
private:
    virtual AiResult _execute(anax::Entity entity);
};

class AiNodeLoop : public AiNodeSequence
{
public:
private:
    virtual AiResult _execute(anax::Entity entity);
};

class AiNodeSelector : public AiNodeComposite
{
public:
private:
    virtual AiResult _execute(anax::Entity entity);
};

class AiNodeDecorator : public AiNode
{
public:
    AiNodeDecorator(AiNode* p_decoratee) : m_up_decoratee(p_decoratee)
    {
    }

protected:
    virtual AiResult _execute(anax::Entity entity);
    virtual void _success(anax::Entity entity);
    virtual void _failure(anax::Entity entity);
    std::unique_ptr<AiNode> m_up_decoratee;
};

class AiNodeDecoratorInvert : public AiNodeDecorator
{
public:
    AiNodeDecoratorInvert(AiNode* p_decoratee) : AiNodeDecorator(p_decoratee)
    {
    }

protected:
    virtual AiResult _execute(anax::Entity entity);
};

class AiNodeMoveTo : public AiNode
{
public:
    AiNodeMoveTo(double pos_x, double pos_y, double tolerance);

private:
    double m_pos_x, m_pos_y, m_tolerance;
    virtual AiResult _execute(anax::Entity entity);
};

struct AiSystem : anax::System<anax::Requires<components::AiComponent>>
{
    AiSystem();

    void update();

private:
    std::shared_ptr<spdlog::logger> m_sp_logger;
};

class AiNodeFollow : public AiNode
{
public:
    AiNodeFollow(
        anax::Entity target, double tolerance, bool follow_x, bool follow_y);

private:
    virtual AiResult _execute(anax::Entity entity);

    anax::Entity m_target;
    double m_tolerance;
    bool m_follow_x, m_follow_y;
};

} // namespace components
#endif /* SYSTEMS_AI_HPP */
