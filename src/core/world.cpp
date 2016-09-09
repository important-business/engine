#include "world.hpp"

#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"

#include <anax/anax.hpp>

namespace core
{
/* const uint16_t LEVEL_SIZE_X = 5; */
/* const uint16_t LEVEL_SIZE_Y = 5; */
#define LEVEL_SIZE_X 5
#define LEVEL_SIZE_Y 5

const char LEVEL_DATA[LEVEL_SIZE_X * LEVEL_SIZE_Y] = {'|',
    '-',
    '-',
    '-',
    '-',
    '|',
    ' ',
    '|',
    ' ',
    ' ',
    '|',
    ' ',
    '|',
    ' ',
    ' ',
    '|',
    ' ',
    ' ',
    ' ',
    ' ',
    '|',
    '-',
    '-',
    '-',
    '-'};

anax::Entity goose_factory(anax::World& world,
    core::ResourceManagerTexture& texture_manager,
    float pos_x,
    float pos_y)
{
    auto entity = world.createEntity();
    auto& sprite = entity.addComponent<components::TextureComponent>();
    sprite.p_texture =
        texture_manager.get(std::string("resources/angry_goose_head.png"));

    (void)entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, 128.0f, 128.0f, 0.0f, false, true);
    (void)entity.addComponent<components::VelocityComponent>();

    entity.activate();

    return entity;
}

anax::Entity player_factory(anax::World& world,
    core::ResourceManagerTexture& texture_manager,
    float pos_x,
    float pos_y)
{
    auto entity = world.createEntity();
    auto& sprite = entity.addComponent<components::TextureComponent>();
    sprite.p_texture =
        texture_manager.get(std::string("resources/angry_goose_head.png"));

    (void)entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, 128.0f, 128.0f, 0.0f, false, true);
    (void)entity.addComponent<components::PlayerComponent>();
    (void)entity.addComponent<components::VelocityComponent>();

    entity.activate();
    return entity;
}

void World::init(Uint32 sdl_render_flags)
{
    // TODO(Keegan): Remove SDL window initialization from world?
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    m_up_anax_world = std::make_unique<anax::World>();
    m_up_render_system =
        std::make_unique<systems::Render>(m_p_window, sdl_render_flags);

    m_up_movement_system = std::make_unique<systems::Movement>();
    m_up_player_input_system = std::make_unique<systems::PlayerInput>();

    m_up_texture_manager = std::make_unique<core::ResourceManagerTexture>();

    m_up_texture_manager->set_default_renderer(
        m_up_render_system->get_renderer());

    (void)player_factory(*m_up_anax_world, *m_up_texture_manager, 100.0, 300.0);
    (void)goose_factory(*m_up_anax_world, *m_up_texture_manager, 100.0, 200.0);
    (void)goose_factory(*m_up_anax_world, *m_up_texture_manager, 100.0, 100.0);

    m_up_anax_world->addSystem(*m_up_render_system);
    m_up_anax_world->addSystem(*m_up_movement_system);
    m_up_anax_world->addSystem(*m_up_player_input_system);

    m_up_level = std::make_unique<core::Level>(LEVEL_SIZE_X, LEVEL_SIZE_Y);
    m_up_level->print();
    m_up_level->load(LEVEL_DATA, LEVEL_SIZE_X, LEVEL_SIZE_Y);
    m_up_level->print();
}

void World::execute(float dt)
{
    m_up_anax_world->refresh();
    m_up_player_input_system->update();
    m_up_movement_system->update(dt);
    m_up_render_system->render();
    handle_input();
}

void World::deinit()
{
    m_up_anax_world->clear();
    m_up_anax_world->refresh();

    m_up_anax_world.release();
    m_up_render_system.release();
    m_up_player_input_system.release();
    m_up_movement_system.release();
    m_up_texture_manager->unload_unused();
    m_up_texture_manager.release();
}

void World::handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            m_p_application->quit();
        }
        else
        {
        }
    }
}

} // namespace core
