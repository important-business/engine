#include <anax/anax.hpp>
#include "world.hpp"
#include "components/transform.hpp"
#include "components/render.hpp"
#include "components/velocity.hpp"

using namespace core;

anax::Entity goose_factory(anax::World* pworld,
    core::ResourceManagerTexture* ptexturemanager,
    float posx,
    float posy)
{
    auto pgoose_texture =
        ptexturemanager->get(std::string("resources/angry_goose_head.png"));

    auto player = pworld->createEntity();

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
    panaxworld = new anax::World{};
    prendersystem = new systems::RenderSystem{pwindow, sdlrenderflags};

    pmovementsystem = new systems::MovementSystem{};

    core::ResourceManagerTexture texturemanager{};

    texturemanager.setDefaultRenderer(prendersystem->getRenderer());

    (void)goose_factory(panaxworld, &texturemanager, 100.0, 300.0);

    (void)goose_factory(panaxworld, &texturemanager, 100.0, 200.0);

    (void)goose_factory(panaxworld, &texturemanager, 100.0, 100.0);

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
    /* goose1.kill(); */
    /* goose2.kill(); */
    /* // Refresh call needed for entity and components to be deleted */
    /* panaxworld->refresh(); */
    /* texturemanager.unloadUnused(); */
    /* goose3.kill(); */
    /* panaxworld->refresh(); */
    /* texturemanager.unloadUnused(); */
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
