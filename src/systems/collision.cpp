#include "systems/collision.hpp"
#include "components/id.hpp"
#include "core/exception.hpp"

#include <anax/System.hpp>

#include <cassert>
#include <cmath>

namespace systems
{

Collision::Collision()
{
    m_sp_logger = core::logging_get_logger("collision");
}

Manifold* systems::Collision::check_collision(
    anax::Entity& e1, anax::Entity& e2)
{
    throw_if_missing_component<components::TransformComponent>(e1);
    auto& transform1 = e1.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::TransformComponent>(e2);
    auto& transform2 = e2.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::Collision>(e1);
    auto& bbox1 = e1.getComponent<components::Collision>().bounding_box;

    throw_if_missing_component<components::Collision>(e2);
    auto& bbox2 = e2.getComponent<components::Collision>().bounding_box;

    Manifold* p_manifold = nullptr;
    // TODO(Keegan): Don't ignore bounding box x/y positions)
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

Manifold* Collision::check_level_collision(
    anax::Entity& e, core::Level* p_level)
{
    throw_if_missing_component<components::TransformComponent>(e);
    auto& transform = e.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::Collision>(e);
    auto& bbox = e.getComponent<components::Collision>().bounding_box;

    auto p_manifold = new Manifold();
    bool collision = false;

    int level_x, level_y, level_layers;
    assert(p_level != nullptr);
    p_level->get_size(level_x, level_y, level_layers);
    level_x *= p_level->get_scale();
    level_y *= p_level->get_scale();
    if (transform.pos_x - bbox.w / 2.0f < 0.0f)
    {
        collision = true;
        p_manifold->normal.x = -1.0f;
        p_manifold->penetration.x = 0.0 - (transform.pos_x - bbox.w / 2.0f);
    }
    else if (transform.pos_x + bbox.w / 2.0f > level_x)
    {
        collision = true;
        p_manifold->normal.x = 1.0f;
        p_manifold->penetration.x = level_x - transform.pos_x + bbox.w / 2.0f;
    }
    if (transform.pos_y - bbox.h / 2.0f < 0.0f)
    {
        collision = true;
        p_manifold->normal.y = -1.0f;
        p_manifold->penetration.y = 0.0f - transform.pos_y - bbox.h / 2.0f;
    }
    else if (transform.pos_y + bbox.h / 2.0f > level_y)
    {
        collision = true;
        p_manifold->normal.y = 1.0f;
        p_manifold->penetration.y = level_y - transform.pos_y + bbox.h / 2.0f;
    }
    if (collision)
    {
        return p_manifold;
    }
    else
    {
        delete p_manifold;
        return nullptr;
    }
}

void Collision::resolve_collision(
    anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold)
{
    assert(p_manifold != nullptr);

    if (!e1.hasComponent<components::PhysicsComponent>() ||
        !e2.hasComponent<components::PhysicsComponent>())
    {
        return;
    }
    auto& physics2 = e2.getComponent<components::PhysicsComponent>();
    auto& physics1 = e1.getComponent<components::PhysicsComponent>();

    throw_if_missing_component<components::TransformComponent>(e1);
    auto& transform1 = e1.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::TransformComponent>(e2);
    auto& transform2 = e2.getComponent<components::TransformComponent>();
    do_resolve_collision(
        &physics1, &transform1, &physics2, &transform2, p_manifold);
}

void Collision::resolve_collision(anax::Entity& e1, Manifold* p_manifold)
{
    const float level_mass = 0.0f;
    const float level_friction = 0.0f;
    const float level_restitution = 0.0f;
    const float level_vel_x = 0.0f;
    const float level_vel_y = 0.0f;
    const float level_force_x = 0.0f;
    const float level_force_y = 0.0f;

    const float level_pos_x = 0.0f;
    const float level_pos_y = 0.0f;
    const float level_size_x = 0.0f;
    const float level_size_y = 0.0f;
    const float level_rotation = 0.0f;
    const bool level_flip_vert = false;
    const bool level_flip_horiz = false;
    assert(p_manifold != nullptr);

    if (!e1.hasComponent<components::PhysicsComponent>())
    {
        return;
    }
    auto& physics1 = e1.getComponent<components::PhysicsComponent>();
    auto physics2 = components::PhysicsComponent{level_mass,
        level_friction,
        level_restitution,
        level_vel_x,
        level_vel_y,
        level_force_x,
        level_force_y};

    throw_if_missing_component<components::TransformComponent>(e1);
    auto& transform1 = e1.getComponent<components::TransformComponent>();
    auto transform2 = components::TransformComponent{level_pos_x,
        level_pos_y,
        level_size_x,
        level_size_y,
        level_rotation,
        level_flip_vert,
        level_flip_horiz};

    do_resolve_collision(
        &physics1, &transform1, &physics2, &transform2, p_manifold);
}

void Collision::do_resolve_collision(components::PhysicsComponent* p_physics1,
    components::TransformComponent* p_transform1,
    components::PhysicsComponent* p_physics2,
    components::TransformComponent* p_transform2,
    Manifold* p_manifold)
{
    assert(p_manifold != nullptr);
    assert(p_physics1 != nullptr);
    assert(p_transform1 != nullptr);
    assert(p_physics2 != nullptr);
    assert(p_transform2 != nullptr);

    float vel_dx = p_physics2->velocity.x - p_physics1->velocity.x;
    float vel_dy = p_physics2->velocity.y - p_physics1->velocity.y;

    float vel_normal =
        vel_dx * p_manifold->normal.x + vel_dy * p_manifold->normal.y;

    m_sp_logger->debug("Normal velocity is {}", vel_normal);
    m_sp_logger->debug("rel velocity is x{}, y{}", vel_dx, vel_dy);
    if (vel_normal > 0)
    {
        m_sp_logger->debug("Objects are moving away");
        return;
    }
    // TODO(Keegan): Add restitution calculation
    const float restitution =
        std::max(p_physics1->restitution, p_physics2->restitution);

    float impulse = -(1.0f * restitution) * vel_normal;

    // TODO(Keegan): Add mass consideration? Or keep in movement?

    float impulse_x = impulse * p_manifold->normal.x;
    float impulse_y = impulse * p_manifold->normal.y;

    m_sp_logger->debug("impulse is x{}, y{}", impulse_x, impulse_y);

    p_physics1->force.x -= impulse_x;
    p_physics2->force.x += impulse_x;

    p_physics1->force.y -= impulse_y;
    p_physics2->force.y += impulse_y;

    m_sp_logger->debug(
        "entity1 force is x{}, y{}", p_physics1->force.x, p_physics1->force.y);
    m_sp_logger->debug(
        "entity2 force is x{}, y{}", p_physics2->force.x, p_physics2->force.y);

    const float positional_percent = 0.2;
    const float positional_slop = 0.01;
    float x_correction =
        std::max(p_manifold->penetration.x - positional_slop, 0.0f) /
        (p_physics1->inv_mass + p_physics2->inv_mass) * positional_percent *
        p_manifold->normal.x;
    float y_correction =
        std::max(p_manifold->penetration.y - positional_slop, 0.0f) /
        (p_physics1->inv_mass + p_physics2->inv_mass) * positional_percent *
        p_manifold->normal.y;
    p_transform1->pos_x -= p_physics1->inv_mass * x_correction;
    p_transform1->pos_y -= p_physics1->inv_mass * y_correction;
    p_transform2->pos_x += p_physics2->inv_mass * x_correction;
    p_transform2->pos_y += p_physics2->inv_mass * y_correction;
}

void Collision::update(core::Level* p_level)
{
    auto colliders = getEntities();

    for (std::size_t i = 0; i < colliders.size(); ++i)
    {
        auto& e1 = colliders[i];

        // No hasComponent check needed since system filters entities
        if (!e1.getComponent<components::Collision>().can_cause_events)
        {
            continue;
        }
        if (p_level != nullptr)
        {
            auto up_manifold =
                std::unique_ptr<Manifold>(check_level_collision(e1, p_level));
            if (up_manifold)
            {
                resolve_collision(e1, up_manifold.get());
            }
        }
        for (std::size_t j = i + 1; j < colliders.size(); ++j)
        {
            auto& e2 = colliders[j];
            auto up_manifold =
                std::unique_ptr<Manifold>(check_collision(e1, e2));
            if (up_manifold)
            {
                resolve_collision(e1, e2, up_manifold.get());
                check_trigger(e1, e2, up_manifold.get());
            }
        }
    }
}

void Collision::check_trigger(
    anax::Entity& e1, anax::Entity& e2, Manifold* p_manifold)
{
    if (e1.hasComponent<components::Trigger>())
    {
        auto msg = e1.getComponent<components::Trigger>().trigger_msg;
        m_trigger_signal.emit(msg, e2);
    }
    else if (e2.hasComponent<components::Trigger>())
    {
        auto msg = e2.getComponent<components::Trigger>().trigger_msg;
        m_trigger_signal.emit(msg, e1);
    }
}

} // namespace systems
