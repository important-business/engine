#ifndef SYSTEMS_HOCKEY_HPP
#define SYSTEMS_HOCKEY_HPP

#include "systems.hpp"

#include "components/player.hpp"
#include "components/velocity.hpp"
#include "common/logging.hpp"

#include <anax/System.hpp>
#include <wink/signal.hpp>
#include <wink/slot.hpp>

namespace systems
{

// TODO(Keegan): Make systems classes)
struct Hockey : anax::System<anax::Requires<components::PlayerComponent,
                         components::PhysicsComponent>>
{
public:
    explicit Hockey(int score_limit=5);
    void update();
    void check_trigger(std::string msg, anax::Entity e_target);

private:
    int m_score_a, m_score_b, m_score_limit;

    std::shared_ptr<spdlog::logger> m_sp_logger;

};
}

#endif /* SYSTEMS_HOCKEY_HPP */
