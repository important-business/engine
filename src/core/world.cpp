#include "world.hpp"
#include "data.hpp"

#include "components/camera.hpp"
#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "components/collision.hpp"

#include <anax/anax.hpp>
#include <iostream>

namespace core
{
/* const uint16_t LEVEL_SIZE_X = 5; */
/* const uint16_t LEVEL_SIZE_Y = 5; */
#define LEVEL_SIZE_X 5
#define LEVEL_SIZE_Y 5

const LevelTile LEVEL_TILE_WALL = {true, 0, 255, 0};
const LevelTile LEVEL_TILE_FLOOR = {false, 0, 0, 0};

// TODO(keegan): Try initialize the level tile thing here
std::map<std::string, const LevelTile*> LEVEL_TILES{
    {"wall", &LEVEL_TILE_WALL}, {"floor", &LEVEL_TILE_FLOOR}};

const std::string LEVEL_DATA[LEVEL_SIZE_X * LEVEL_SIZE_Y] = {"wall",
    "wall",
    "wall",
    "wall",
    "wall",
    "wall",
    "floor",
    "wall",
    "floor",
    "floor",
    "wall",
    "floor",
    "wall",
    "floor",
    "floor",
    "wall",
    "floor",
    "floor",
    "floor",
    "floor",
    "wall",
    "wall",
    "wall",
    "wall",
    "wall"};

const float LEVEL_DEFAULT_SCALE = 100.0;

anax::Entity camera_factory(
    anax::World& world, anax::Entity target, float pos_x, float pos_y)
{
    auto entity = world.createEntity();

    (void)entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, 0.0f, 0.0f, 0.0f, false, true);
    (void)entity.addComponent<components::VelocityComponent>(
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    (void)entity.addComponent<components::CameraComponent>(target, 1.0f);

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
    (void)entity.addComponent<components::VelocityComponent>(
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    (void)entity.addComponent<components::Collision>(
        0, 0, (int)transform.size_x, (int)transform.size_y, true);

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
    (void)entity.addComponent<components::PlayerComponent>(30.0f, 500.0f);
    (void)entity.addComponent<components::VelocityComponent>(
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    (void)entity.addComponent<components::Collision>(
        0, 0, (int)transform.size_x, (int)transform.size_y, true);

    entity.activate();
    return entity;
}

void World::init(Uint32 sdl_render_flags)
{
    // TODO(Keegan): Remove SDL window initialization from world?
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    m_up_anax_world = std::make_unique<anax::World>();
    m_up_camera_system = std::make_unique<systems::Camera>();

    m_up_texture_manager = std::make_unique<core::ResourceManagerTexture>();

    m_up_render_system = std::make_unique<systems::Render>(m_p_window,
        sdl_render_flags,
        *m_up_camera_system,
        m_up_texture_manager.get());

    m_up_movement_system = std::make_unique<systems::Movement>();
    m_up_player_input_system = std::make_unique<systems::PlayerInput>();
    m_up_collision_system = std::make_unique<systems::Collision>();

    m_up_texture_manager->set_default_renderer(
        m_up_render_system->get_renderer());

    auto datareader = DataReader("data/world.json");
    datareader.makeEntities(*m_up_anax_world);

    m_up_anax_world->addSystem(*m_up_render_system);
    m_up_anax_world->addSystem(*m_up_camera_system);
    m_up_anax_world->addSystem(*m_up_movement_system);
    m_up_anax_world->addSystem(*m_up_player_input_system);
    m_up_anax_world->addSystem(*m_up_collision_system);

    m_up_collision_system->add_listener(*this);

    m_up_level = std::make_unique<core::Level>(
        LEVEL_SIZE_X, LEVEL_SIZE_Y, LEVEL_DEFAULT_SCALE);
    /* m_up_level->print(); */
    m_up_level->load(LEVEL_DATA, LEVEL_SIZE_X, LEVEL_SIZE_Y, LEVEL_TILES);
    /* m_up_level->print(); */
}

void World::on_collision_occured(
    anax::Entity& e1, anax::Entity& e2, double delta_time)
{
    m_sp_logger->info("collision occurred\n");
    /* auto& velocity = e1.getComponent<components::VelocityComponent>(); */
    /* auto& transform = e1.getComponent<components::TransformComponent>(); */

    /* auto distance_moved_x = -velocity.x * delta_time * 2; */
    /* auto distance_moved_y = -velocity.y * delta_time * 2; */

    /* transform.pos_x += distance_moved_x; */
    /* transform.pos_y += distance_moved_y; */
}

void World::execute(float dt)
{
    m_up_anax_world->refresh();
    m_up_player_input_system->update();
    m_up_camera_system->update();
    m_up_collision_system->update(dt);
    m_up_movement_system->update(dt);
    m_up_render_system->render(m_up_level.get());
}

void World::deinit()
{
    m_up_anax_world->clear();
    m_up_anax_world->refresh();

    m_up_anax_world.release();
    m_up_render_system.release();
    m_up_camera_system.release();
    m_up_player_input_system.release();
    m_up_movement_system.release();
    m_up_texture_manager->unload_unused();
    m_up_texture_manager.release();
}

} // namespace core
