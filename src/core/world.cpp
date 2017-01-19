#include "world.hpp"
#include "data.hpp"

#include "components/ai.hpp"
#include "components/camera.hpp"
#include "components/collision.hpp"
#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"

#include <anax/anax.hpp>

namespace core
{

anax::Entity camera_factory(anax::World& world, float pos_x, float pos_y)
{
    auto entity = world.createEntity();

    (void)entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, 0.0f, 0.0f, 0.0f, false, true);
    (void)entity.addComponent<components::PhysicsComponent>(
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    (void)entity.addComponent<components::CameraComponent>(1.0f);

    entity.activate();
    return entity;
}

anax::Entity goose_factory(anax::World& world,
    core::ResourceManagerTexture& texture_manager,
    float pos_x,
    float pos_y)
{
    auto entity = world.createEntity();
    (void)entity.addComponent<components::TextureComponent>(
        std::string("resources/angry_goose_head.png"));

    auto transform = entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, 128.0f, 128.0f, 0.0f, false, true);
    (void)entity.addComponent<components::PhysicsComponent>(
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    (void)entity.addComponent<components::Collision>(
        0.0f, 0.0f, transform.size_x, transform.size_y, true);

    entity.activate();

    return entity;
}

anax::Entity player_factory(anax::World& world,
    core::ResourceManagerTexture& texture_manager,
    float pos_x,
    float pos_y)
{
    auto entity = world.createEntity();
    (void)entity.addComponent<components::TextureComponent>(
        std::string("resources/angry_goose_head.png"));

    auto transform = entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, 128.0f, 128.0f, 0.0f, false, true);
    (void)entity.addComponent<components::PlayerComponent>();
    (void)entity.addComponent<components::PhysicsComponent>(
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    (void)entity.addComponent<components::Collision>(
        0.0f, 0.0f, transform.size_x, transform.size_y, true);

    entity.activate();
    return entity;
}

void World::init()
{
    m_up_anax_world = std::make_unique<anax::World>();
    m_up_camera_system = std::make_unique<systems::Camera>();

    m_up_texture_manager = std::make_unique<core::ResourceManagerTexture>();

    m_up_render_system = std::make_unique<systems::Render>(
        m_p_window, m_config, *m_up_camera_system, m_up_texture_manager.get());

    m_up_ai_system = std::make_unique<systems::AiSystem>();

    m_up_movement_system = std::make_unique<systems::Movement>();
    m_up_player_input_system = std::make_unique<systems::PlayerInput>();
    m_up_collision_system = std::make_unique<systems::Collision>();

    m_up_hockey_system = std::make_unique<systems::Hockey>();

    m_up_texture_manager->set_default_renderer(
        m_up_render_system->get_renderer());

    auto datareader = DataReader("data/world.json");
    datareader.makeEntities(*m_up_anax_world);

    auto levelreader = LevelReader("data/level/frozen_lake.json");
    levelreader.build_level(m_up_level);

    m_up_anax_world->addSystem(*m_up_ai_system);
    m_up_anax_world->addSystem(*m_up_render_system);
    m_up_anax_world->addSystem(*m_up_camera_system);
    m_up_anax_world->addSystem(*m_up_movement_system);
    m_up_anax_world->addSystem(*m_up_player_input_system);
    m_up_anax_world->addSystem(*m_up_collision_system);

    m_up_collision_system->m_trigger_signal.connect(
        m_up_hockey_system.get(), &systems::Hockey::check_trigger);

    m_up_player_input_system->m_movement_signal.connect(
        m_up_movement_system.get(), &systems::Movement::move_actor);
    m_up_ai_system->m_movement_signal.connect(
        m_up_movement_system.get(), &systems::Movement::move_actor);
}

void World::execute(float dt)
{
    m_up_anax_world->refresh();
    m_up_render_system->render(m_up_level.get());
}

void World::execute_fixed(float dt)
{
    m_up_anax_world->refresh();
    m_up_player_input_system->update();
    m_up_ai_system->update();
    m_up_collision_system->update(m_up_level.get());
    m_up_movement_system->update(dt);
    m_up_render_system->render(m_up_level.get());
}

void World::deinit()
{
    m_up_anax_world->clear();
    m_up_anax_world->refresh();

    m_up_anax_world.release();
    m_up_ai_system.release();
    m_up_render_system.release();
    m_up_camera_system.release();
    m_up_player_input_system.release();
    m_up_movement_system.release();
    m_up_texture_manager->unload_unused();
    m_up_texture_manager.release();
}

} // namespace core
