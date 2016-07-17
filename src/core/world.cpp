#include <anax/anax.hpp>
#include "world.hpp"
#include "components/transform.hpp"
#include "components/render.hpp"
#include "components/velocity.hpp"

using namespace core;

anax::Entity goose_factory(anax::World& world,
    core::ResourceManagerTexture& texturemanager,
    float posx,
    float posy)
{
    auto pgoose_texture =
        texturemanager.get(std::string("resources/angry_goose_head.png"));

    auto player = world.createEntity();

    auto& sprite = player.addComponent<components::TextureComponent>();
    sprite.ptexture = pgoose_texture;

    (void)player.addComponent<components::TransformComponent>(
        posx, posy, 128.0f, 128.0f, 0.0f, false, true);

    auto& velocity = player.addComponent<components::VelocityComponent>();
    velocity.x = 100;
    velocity.y = 100;

    player.activate();

    return player;
}

void World::init(Uint32 sdlrenderflags)
{
    // TODO: Remove SDL window initialization from world?
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    panaxworld = std::make_unique<anax::World>();
    prendersystem = std::make_unique<systems::RenderSystem>(pwindow, sdlrenderflags);

    pmovementsystem = std::make_unique<systems::MovementSystem>();

    ptexturemanager = std::make_unique<core::ResourceManagerTexture>();

    ptexturemanager->setDefaultRenderer(prendersystem->getRenderer());

    (void)goose_factory(*panaxworld, *ptexturemanager, 100.0, 300.0);

    (void)goose_factory(*panaxworld, *ptexturemanager, 100.0, 200.0);

    (void)goose_factory(*panaxworld, *ptexturemanager, 100.0, 100.0);

    panaxworld->addSystem(*prendersystem);
    panaxworld->addSystem(*pmovementsystem);
    toquit = false;
}

bool World::isToQuit()
{
    return toquit;
}

void World::execute(float dt)
{
    if (not toquit)
    {
        panaxworld->refresh();
        prendersystem->render();
        pmovementsystem->update(dt);
        handle_input();
    }
}

void World::deinit()
{
    panaxworld->clear();
    panaxworld->refresh();

    panaxworld.release();
    prendersystem.release();
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
            toquit = true;
        }
        else
        {
        }
    }
}
