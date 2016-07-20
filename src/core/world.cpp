#include <anax/anax.hpp>
#include "world.hpp"
#include "components/transform.hpp"
#include "components/render.hpp"
#include "components/player.hpp"
#include "components/velocity.hpp"

using namespace core;

anax::Entity goose_factory(anax::World& world,
    core::ResourceManagerTexture& texturemanager,
    float posx,
    float posy)
{
    auto pgoose_texture =
        texturemanager.get(std::string("resources/angry_goose_head.png"));

    auto entity = world.createEntity();

    auto& sprite = entity.addComponent<components::TextureComponent>();
    sprite.ptexture = pgoose_texture;

    (void)entity.addComponent<components::TransformComponent>(
        posx, posy, 128.0f, 128.0f, 0.0f, false, true);

    (void)entity.addComponent<components::PlayerComponent>();

    (void)entity.addComponent<components::VelocityComponent>();

    entity.activate();

    return entity;
}

void World::init(Uint32 sdlrenderflags)
{
    // TODO: Remove SDL window initialization from world?
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    panaxworld = std::make_unique<anax::World>();
    prendersystem =
        std::make_unique<systems::RenderSystem>(pwindow, sdlrenderflags);

    pmovementsystem = std::make_unique<systems::MovementSystem>();
    pplayerinputsystem = std::make_unique<systems::PlayerInputSystem>();

    ptexturemanager = std::make_unique<core::ResourceManagerTexture>();

    ptexturemanager->setDefaultRenderer(prendersystem->getRenderer());

    (void)goose_factory(*panaxworld, *ptexturemanager, 100.0, 300.0);

    (void)goose_factory(*panaxworld, *ptexturemanager, 100.0, 200.0);

    (void)goose_factory(*panaxworld, *ptexturemanager, 100.0, 100.0);

    panaxworld->addSystem(*prendersystem);
    panaxworld->addSystem(*pmovementsystem);
    panaxworld->addSystem(*pplayerinputsystem);
}

void World::execute(float dt)
{
    panaxworld->refresh();
    pplayerinputsystem->update(dt);
    pmovementsystem->update(dt);
    prendersystem->render();
    handle_input();
}

void World::deinit()
{
    panaxworld->clear();
    panaxworld->refresh();

    panaxworld.release();
    prendersystem.release();
    pplayerinputsystem.release();
    pmovementsystem.release();
    ptexturemanager->unloadUnused();
    ptexturemanager.release();
}

void World::handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            papplication->quit();
        }
        else
        {
        }
    }
}
