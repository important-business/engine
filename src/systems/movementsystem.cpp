#include "movementsystem.hpp"
#include <iostream>

using namespace systems;

void MovementSystem::update(double deltaTime)
{
    auto entities = getEntities();
    for (auto& entity : entities)
    {
        auto& transform = entity.getComponent<components::TransformComponent>();
        auto& velocity = entity.getComponent<components::VelocityComponent>();

        auto distance_moved_x = velocity.x * (float)deltaTime;
        auto distance_moved_y = velocity.y * (float)deltaTime;

        transform.pos_x += distance_moved_x;
        transform.pos_y += distance_moved_y;
    }
}
