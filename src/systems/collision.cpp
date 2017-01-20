#include "systems/collision.hpp"
#include "common/exception.hpp"
#include "components/id.hpp"

#include <anax/System.hpp>

#include <cassert>
#include <cmath>

namespace systems
{

Collision::Collision() : m_sp_logger(common::logging_get_logger("collision"))
{
}

Manifold::operator bool()
{
    return normal.x != 0.0f || normal.y != 0.0f;
}

Manifold Collision::check_rect_collision(
    const common::Rectangle& rect1, const common::Rectangle& rect2)
{
    Manifold manifold;
    // TODO(Keegan): Don't ignore bounding box x/y positions)
    float pos_dx = rect1.x - rect2.x;
    float pos_dy = rect1.y - rect2.y;

    float x_overlap = rect1.w / 2.0f + rect2.w / 2.0f - std::abs(pos_dx);

    if (x_overlap > 0.0f)
    {
        float y_overlap = rect1.h / 2.0f + rect2.h / 2.0f - std::abs(pos_dy);
        if (y_overlap > 0.0f)
        {
            m_sp_logger->trace(
                " collision - x overlap {} y overlap {}", x_overlap, y_overlap);
            if (x_overlap < y_overlap)
            {
                if (pos_dx < 0)
                {
                    m_sp_logger->trace("Normal in -y");
                    manifold.normal.x = 1;
                    manifold.normal.y = 0;
                }
                else
                {
                    m_sp_logger->trace("Normal in +y");
                    manifold.normal.x = -1;
                    manifold.normal.y = 0;
                }
                manifold.penetration.x = x_overlap;
            }
            else
            {
                if (pos_dy < 0)
                {
                    m_sp_logger->trace("Normal in -x");
                    manifold.normal.x = 0;
                    manifold.normal.y = 1;
                }
                else
                {
                    m_sp_logger->trace("Normal in +x");
                    manifold.normal.x = 0;
                    manifold.normal.y = -1;
                }
                manifold.penetration.y = y_overlap;
            }
        }
    }
    return manifold;
}

Manifold systems::Collision::check_collision(anax::Entity& e1, anax::Entity& e2)
{
    throw_if_missing_component<components::TransformComponent>(e1);
    auto& transform1 = e1.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::TransformComponent>(e2);
    auto& transform2 = e2.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::Collision>(e1);
    auto& bbox1 = e1.getComponent<components::Collision>().bounding_box;

    throw_if_missing_component<components::Collision>(e2);
    auto& bbox2 = e2.getComponent<components::Collision>().bounding_box;

    common::Rectangle rect1{transform1.pos_x + bbox1.x,
        transform1.pos_y + bbox1.y,
        bbox1.w,
        bbox1.h};
    common::Rectangle rect2{transform2.pos_x + bbox2.x,
        transform2.pos_y + bbox2.y,
        bbox2.w,
        bbox2.h};

    return check_rect_collision(rect1, rect2);
}

Manifold Collision::check_level_collision(anax::Entity& e, core::Level* p_level)
{
    throw_if_missing_component<components::TransformComponent>(e);
    auto& transform = e.getComponent<components::TransformComponent>();

    throw_if_missing_component<components::Collision>(e);
    auto& bbox = e.getComponent<components::Collision>().bounding_box;

    auto manifold = Manifold();
    bool collision = false;

    int level_x1, level_y1, level_x2, level_y2;
    assert(p_level != nullptr);
    float world_x1 = transform.pos_x - (bbox.w / 2.0f);
    float world_y1 = transform.pos_y - (bbox.h / 2.0f);
    float world_x2 = transform.pos_x + (bbox.w / 2.0f);
    float world_y2 = transform.pos_y + (bbox.h / 2.0f);
    /* m_sp_logger->debug( */
    /*     "World coordinates {},{}-{},{}", world_x1, world_y1, world_x2,
     * world_y2); */
    p_level->get_tile_coords(world_x1, world_y1, level_x1, level_y1);
    p_level->get_tile_coords(world_x2, world_y2, level_x2, level_y2);
    /* m_sp_logger->debug( */
    /* "Checking tiles {},{}-{},{}", level_x1, level_y1, level_x2, level_y2); */
    for (int x = level_x1; x <= level_x2; x++)
    {
        for (int y = level_y1; y <= level_y2; y++)
        {
            if (p_level->get_collision(x, y))
            {

                float world_x, world_y, tile_size;
                p_level->get_world_coords(x, y, world_x, world_y);
                tile_size = p_level->get_scale();
                common::Rectangle rect1{transform.pos_x + bbox.x,
                    transform.pos_y + bbox.y,
                    bbox.w,
                    bbox.h};
                common::Rectangle rect2{world_x + tile_size / 2.0f,
                    world_y + tile_size / 2.0f,
                    tile_size,
                    tile_size};

                auto new_manifold = check_rect_collision(rect1, rect2);
                if (new_manifold)
                {
                    m_sp_logger->debug("Collision");
                    collision = true;
                    /* if (new_manifold.penetration >
                     * manifold.penetration) */
                    /* { */
                    /*     manifold = new_manifold; */
                    /* } */
                    manifold.normal += new_manifold.normal;
                    manifold.penetration += new_manifold.penetration;
                }
            }
            m_sp_logger->trace("Checking tile {},{}", x, y);
        }
    }
    if (collision)
    {
        manifold.normal = manifold.normal.normalize();
        m_sp_logger->debug("   Normal:{},{}, penetration:{},{}",
            manifold.normal.x,
            manifold.normal.y,
            manifold.penetration.x,
            manifold.penetration.y);
    }
    return manifold;
}

void Collision::resolve_collision(
    anax::Entity& e1, anax::Entity& e2, const Manifold& manifold)
{
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
        &physics1, &transform1, &physics2, &transform2, manifold);
}

void Collision::resolve_collision(anax::Entity& e1, const Manifold& manifold)
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
        &physics1, &transform1, &physics2, &transform2, manifold);
}

void Collision::do_resolve_collision(components::PhysicsComponent* p_physics1,
    components::TransformComponent* p_transform1,
    components::PhysicsComponent* p_physics2,
    components::TransformComponent* p_transform2,
    const Manifold& manifold)
{
    assert(p_physics1 != nullptr);
    assert(p_transform1 != nullptr);
    assert(p_physics2 != nullptr);
    assert(p_transform2 != nullptr);

    float vel_dx = p_physics2->velocity.x - p_physics1->velocity.x;
    float vel_dy = p_physics2->velocity.y - p_physics1->velocity.y;

    float vel_normal = vel_dx * manifold.normal.x + vel_dy * manifold.normal.y;

    m_sp_logger->trace("Normal velocity is {}", vel_normal);
    m_sp_logger->trace("rel velocity is x{}, y{}", vel_dx, vel_dy);
    if (vel_normal > 0)
    {
        m_sp_logger->trace("Objects are moving away");
        return;
    }
    // TODO(Keegan): Add restitution calculation
    const float restitution =
        std::max(p_physics1->restitution, p_physics2->restitution);

    float impulse = -(1.0f * restitution) * vel_normal;

    // TODO(Keegan): Add mass consideration? Or keep in movement?

    float impulse_x = impulse * manifold.normal.x;
    float impulse_y = impulse * manifold.normal.y;

    m_sp_logger->trace("impulse is x{}, y{}", impulse_x, impulse_y);

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
        std::max(manifold.penetration.x - positional_slop, 0.0f) /
        (p_physics1->inv_mass + p_physics2->inv_mass) * positional_percent *
        manifold.normal.x;
    float y_correction =
        std::max(manifold.penetration.y - positional_slop, 0.0f) /
        (p_physics1->inv_mass + p_physics2->inv_mass) * positional_percent *
        manifold.normal.y;
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
            auto manifold = check_level_collision(e1, p_level);
            if (manifold)
            {
                resolve_collision(e1, manifold);
            }
        }
        for (std::size_t j = i + 1; j < colliders.size(); ++j)
        {
            auto& e2 = colliders[j];
            auto manifold = check_collision(e1, e2);
            if (manifold)
            {
                resolve_collision(e1, e2, manifold);
                check_trigger(e1, e2, manifold);
            }
        }
    }
}

void Collision::check_trigger(
    anax::Entity& e1, anax::Entity& e2, const Manifold& manifold)
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
