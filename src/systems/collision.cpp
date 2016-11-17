#include "systems/collision.hpp"

#include <SDL.h>
#include <anax/System.hpp>

systems::Collision::Listener::~Listener()
{
}

static SDL_Rect get_bounding_box_rect(
    const components::TransformComponent& transformable, const SDL_Rect bbox)
{
    SDL_Rect bounding_box;
    bounding_box.x = transformable.pos_x + bbox.x;
    bounding_box.y = transformable.pos_y + bbox.y;
    bounding_box.w = bbox.w;
    bounding_box.h = bbox.h;
    return bounding_box;
}

static SDL_Rect get_bounding_box_rect(const anax::Entity& entity)
{
    auto& transform = entity.getComponent<components::TransformComponent>();
    auto& bbox = entity.getComponent<components::Collision>().bounding_box;
    return get_bounding_box_rect(transform, bbox);
}

systems::Collision::Collision()
{
    m_sp_logger = core::logging_get_logger("collision");
}

void systems::Collision::update(double delta_time)
{
    auto colliders = getEntities();

    for (std::size_t i = 0; i < colliders.size(); ++i)
    {
        auto& e1 = colliders[i];

        if (!e1.getComponent<components::Collision>().can_cause_events)
        {
            continue;
        }

        const auto rect1 = get_bounding_box_rect(e1);

        for (std::size_t j = i + 1; j < colliders.size(); ++j)
        {
            auto& e2 = colliders[j];
            const auto rect2 = get_bounding_box_rect(e2);
            SDL_Rect intersecting_rect;
            if (SDL_IntersectRect(&rect1, &rect2, &intersecting_rect))
            {
                for (auto& listener : m_listeners)
                {
                    listener->on_collision_occured(e1, e2, delta_time);
                }
            }
        }
    }
}

void systems::Collision::add_listener(Listener& listener)
{
    m_listeners.push_back(&listener);
}

void systems::Collision::remove_listener(Listener& listener)
{
    m_listeners.erase(
        std::remove(m_listeners.begin(), m_listeners.end(), &listener),
        m_listeners.end());
}
