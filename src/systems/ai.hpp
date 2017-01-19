#ifndef SYSTEMS_AI_HPP
#define SYSTEMS_AI_HPP
#include "components/ai.hpp"
#include "common/logging.hpp"
#include "systems.hpp"

#include <anax/System.hpp>
#include <wink/signal.hpp>
#include <wink/slot.hpp>

namespace systems
{

enum AiResult
{
    AI_RESULT_READY,
    AI_RESULT_FAIL,
    AI_RESULT_SUCCESS,
    AI_RESULT_COUNT
};

struct AiSystem;

class AiNode
{
public:
    AiResult execute(anax::Entity entity, AiSystem const& aisystem);
    void success(anax::Entity entity, AiSystem const& aisystem);
    void failure(anax::Entity entity, AiSystem const& aisystem);
    virtual ~AiNode(){}

private:
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);
    virtual void _success(anax::Entity entity, AiSystem const& aisystem);
    virtual void _failure(anax::Entity entity, AiSystem const& aisystem);
};

class AiNodeComposite : public AiNode
{
public:
    void add_child(AiNode* p_ai_node);
    int get_child_count();

protected:
    std::vector<std::unique_ptr<AiNode>> m_v_up_children;
};

class AiNodeSequence : public AiNodeComposite
{
public:
    virtual ~AiNodeSequence(){}
private:
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);
};

class AiNodeLoop : public AiNodeSequence
{
public:
    virtual ~AiNodeLoop(){}
private:
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);
};

class AiNodeSelector : public AiNodeComposite
{
public:
    virtual ~AiNodeSelector(){}
private:
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);
};

class AiNodeDecorator : public AiNode
{
public:
    explicit AiNodeDecorator(AiNode* p_decoratee) : m_up_decoratee(p_decoratee)
    {
    }
    virtual ~AiNodeDecorator(){}

protected:
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);
    virtual void _success(anax::Entity entity, AiSystem const& aisystem);
    virtual void _failure(anax::Entity entity, AiSystem const& aisystem);
    std::unique_ptr<AiNode> m_up_decoratee;
};

class AiNodeDecoratorInvert : public AiNodeDecorator
{
public:
    explicit AiNodeDecoratorInvert(AiNode* p_decoratee) : AiNodeDecorator(p_decoratee)
    {
    }

    virtual ~AiNodeDecoratorInvert(){}
protected:
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);
    virtual void _success(anax::Entity entity, AiSystem const& aisystem);
    virtual void _failure(anax::Entity entity, AiSystem const& aisystem);
};

class AiNodeMoveTo : public AiNode
{
public:
    AiNodeMoveTo(double pos_x, double pos_y, double tolerance);

    virtual ~AiNodeMoveTo(){}

private:
    double m_pos_x, m_pos_y, m_tolerance;
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);
};

struct AiSystem : anax::System<anax::Requires<components::AiComponent>>
{
    AiSystem();

    void update();

    wink::signal<wink::slot<void (anax::Entity, float, float)>> m_movement_signal;

private:
    std::shared_ptr<spdlog::logger> m_sp_logger;
};

class AiNodeFollow : public AiNode
{
public:
    AiNodeFollow(
        anax::Entity target, double tolerance, bool follow_x, bool follow_y);

    virtual ~AiNodeFollow(){}

private:
    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem);

    anax::Entity m_target;
    double m_tolerance;
    bool m_follow_x, m_follow_y;
};

} // namespace components
#endif /* SYSTEMS_AI_HPP */
