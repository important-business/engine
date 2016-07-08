#include "rendersystem.hpp"
#include "components/render.hpp"

#include <iostream>

using namespace systems;

RenderSystem::RenderSystem(std::string title, int posx, int posy, int width, int height, Uint32 windowflags, Uint32 renderflags){
    pwindow =  new sdl::Window {
        title.c_str(),
        posx,
        posy,
        width,
        height,
        SDL_WINDOW_SHOWN
    };

    prenderer = new sdl::Renderer { *pwindow, -1, renderflags };
    prenderer->set_draw_color(150,150,150,100);
}
RenderSystem::~RenderSystem(){

}

void RenderSystem::render(){
    auto entities = getEntities();

    prenderer->clear();
    for(auto& entity : entities)
    {
        auto texturecomponent = entity.getComponent<components::TextureComponent>();
        auto transformcomponent = entity.getComponent<components::TransformComponent>();
        SDL_Rect dstrect = {
            (int)transformcomponent.pos_x,
            (int)transformcomponent.pos_y,
            (int)transformcomponent.size_x,
            (int)transformcomponent.size_y,
        };
        prenderer->copy(*texturecomponent.ptexture, nullptr, &dstrect);
    }
    prenderer->present();
}


sdl::Renderer *RenderSystem::getRenderer() const{
    return prenderer;
}

sdl::Window *RenderSystem::getWindow() const{
    return pwindow;
}
