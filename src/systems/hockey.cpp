#include "systems/hockey.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"

namespace systems
{

Hockey::Hockey(int score_limit)
    : m_sp_logger(common::logging_get_logger("hockey")),
      m_score_a(0),
      m_score_b(0),
      m_score_limit(score_limit)
{
}

void Hockey::check_trigger(std::string msg, anax::Entity e_target)
{
    auto id = e_target.getComponent<components::IdComponent>().id;

    if (id.compare("puck") == 0)
    {
        bool reset = false;
        if (msg.compare("goal_a") == 0)
        {
            m_sp_logger->info("A team scores!");
            m_score_a++;
            reset = true;
            if (m_score_a > m_score_limit)
            {
                m_sp_logger->info("A team wins!");
            }
        }
        else if (msg.compare("goal_b") == 0)
        {
            m_score_b++;
            reset = true;
            m_sp_logger->info("B team scores!");
            if (m_score_b > m_score_limit)
            {
                m_sp_logger->info("B team wins!");
            }
        }

        if (reset)
        {
            m_sp_logger->info("Score is now A:{} B:{}", m_score_a, m_score_b);
            throw_if_missing_component<components::TransformComponent>(
                e_target);
            throw_if_missing_component<components::PhysicsComponent>(e_target);
            auto& transform =
                e_target.getComponent<components::TransformComponent>();
            auto& physics =
                e_target.getComponent<components::PhysicsComponent>();
            transform.pos_x = 800;
            transform.pos_y = 800;
            physics.velocity.x = 0;
            physics.velocity.y = 0;
            physics.force.x = 0;
            physics.force.y = 0;
        }
    }
}
} // namespace systems
