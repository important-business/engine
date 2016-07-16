#include "rendersystem.hpp"
#include "components/render.hpp"

#include <iostream>

using namespace systems;

RenderSystem::RenderSystem(sdl::Window* pwindowtouse, Uint32 renderflags)
{
    pwindow = pwindowtouse;

    prenderer = new sdl::Renderer{*pwindow, -1, renderflags};
    prenderer->set_draw_color(150, 150, 150, 100);
}
RenderSystem::~RenderSystem()
{
}

void RenderSystem::render()
{
    auto entities = getEntities();

    prenderer->clear();
    for (auto& entity : entities)
    {
        auto texturecomponent =
            entity.getComponent<components::TextureComponent>();
        auto transformcomponent =
            entity.getComponent<components::TransformComponent>();
        SDL_Rect dstrect = {
            (int)transformcomponent.pos_x,
            (int)transformcomponent.pos_y,
            (int)transformcomponent.size_x,
            (int)transformcomponent.size_y,
        };
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (transformcomponent.flip_vert)
        {
            flip =
                (SDL_RendererFlip)((SDL_RendererFlip)SDL_FLIP_VERTICAL | flip);
        }
        if (transformcomponent.flip_horiz)
        {
            flip = (SDL_RendererFlip)(
                (SDL_RendererFlip)SDL_FLIP_HORIZONTAL | flip);
        }
        prenderer->copy(*texturecomponent.ptexture,
            nullptr,
            &dstrect,
            (int)transformcomponent.rotation,
            flip);
    }
    prenderer->present();
}

sdl::Renderer* RenderSystem::getRenderer() const
{
    return prenderer;
}

sdl::Window* RenderSystem::getWindow() const
{
    return pwindow;
}
