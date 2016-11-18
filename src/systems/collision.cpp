#include "systems/collision.hpp"

#include <anax/System.hpp>

#include <cmath>

namespace systems
{

Collision::Listener::~Listener()
{
}

Collision::Collision()
{
    m_sp_logger = core::logging_get_logger("collision");
}

Manifold* systems::Collision::check_collision(
    anax::Entity& e1, anax::Entity& e2)
{
    auto& transform1 = e1.getComponent<components::TransformComponent>();
    auto& transform2 = e2.getComponent<components::TransformComponent>();
    auto& bbox1 = e1.getComponent<components::Collision>().bounding_box;
    auto& bbox2 = e2.getComponent<components::Collision>().bounding_box;

    Manifold* p_manifold = nullptr;
    // TODO(Keegan, Don't ignore bounding box x/y positions)
    float pos_dx = transform1.pos_x - transform2.pos_x;
    float pos_dy = transform1.pos_y - transform2.pos_y;

    float x_overlap = bbox1.w / 2.0f + bbox2.w / 2.0f - std::abs(pos_dx);

    if (x_overlap > 0.0f)
    {
        float y_overlap = bbox1.h / 2.0f + bbox2.h / 2.0f - std::abs(pos_dy);
        if (y_overlap > 0.0f)
        {
            m_sp_logger->debug(
                " collision - x overlap {} y overlap {}", x_overlap, y_overlap);
            p_manifold = new Manifold();
            if (x_overlap < y_overlap)
            {
                if (pos_dx < 0)
                {
                    m_sp_logger->debug("Normal in -y");
                    p_manifold->normal.x = 1;
                    p_manifold->normal.y = 0;
                }
                else
                {
                    m_sp_logger->debug("Normal in +y");
                    p_manifold->normal.x = -1;
                    p_manifold->normal.y = 0;
                }
                p_manifold->penetration.x = x_overlap;
            }
            else
            {
                if (pos_dy < 0)
                {
                    m_sp_logger->debug("Normal in -x");
                    p_manifold->normal.x = 0;
                    p_manifold->normal.y = 1;
                }
                else
                {
                    m_sp_logger->debug("Normal in +x");
                    p_manifold->normal.x = 0;
                    p_manifold->normal.y = -1;
                }
                p_manifold->penetration.y = y_overlap;
            }
        }
    }

    return p_manifold;
}

void Collision::resolve_collision(
    anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold)
{
    // TODO(Keegan, Assert p_manifold != nullptr)
    auto& velocity1 = e1.getComponent<components::VelocityComponent>();
    auto& velocity2 = e2.getComponent<components::VelocityComponent>();

    float vel_dx = velocity2.velocity.x - velocity1.velocity.x;
    float vel_dy = velocity2.velocity.y - velocity1.velocity.y;

    float vel_normal =
        vel_dx * p_manifold->normal.x + vel_dy * p_manifold->normal.y;

    m_sp_logger->debug("Normal velocity is {}", vel_normal);
    m_sp_logger->debug("rel velocity is x{}, y{}", vel_dx, vel_dy);
    if (vel_normal > 0)
    {
        m_sp_logger->debug("Objects are moving away");
        return;
    }
    // TODO(Keegan "Add restitution calculation)
    const float restitution = 1.0f;

    float impulse = -(1.0f * restitution) * vel_normal;

    // TODO(Keegan "Add mass consideration? Or keep in movement?

    float impulse_x = impulse * p_manifold->normal.x;
    float impulse_y = impulse * p_manifold->normal.y;

    m_sp_logger->debug("impulse is x{}, y{}", impulse_x, impulse_y);

    velocity1.force.x -= impulse_x;
    velocity2.force.x += impulse_x;

    velocity1.force.y -= impulse_y;
    velocity2.force.y += impulse_y;

    m_sp_logger->debug(
        "entity1 force is x{}, y{}", velocity1.force.x, velocity1.force.y);
    m_sp_logger->debug(
        "entity2 force is x{}, y{}", velocity2.force.x, velocity2.force.y);
}

void Collision::update(double delta_time)
{
    auto colliders = getEntities();

    for (std::size_t i = 0; i < colliders.size(); ++i)
    {
        auto& e1 = colliders[i];

        if (!e1.getComponent<components::Collision>().can_cause_events)
        {
            continue;
        }

        for (std::size_t j = i + 1; j < colliders.size(); ++j)
        {
            auto& e2 = colliders[j];
            auto up_manifold =
                std::unique_ptr<Manifold>(check_collision(e1, e2));
            if (up_manifold)
            {
                resolve_collision(e1, e2, up_manifold.get());
                for (auto& listener : m_listeners)
                {
                    listener->on_collision_occured(e1, e2, up_manifold.get());
                }
            }
        }
    }
}

void Collision::add_listener(Listener& listener)
{
    m_listeners.push_back(&listener);
}

void Collision::remove_listener(Listener& listener)
{
    m_listeners.erase(
        std::remove(m_listeners.begin(), m_listeners.end(), &listener),
        m_listeners.end());
}

} // namespace systems
