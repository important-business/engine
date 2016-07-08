#ifndef SYSTEMS_RENDERSYSTEM_HPP
#define SYSTEMS_RENDERSYSTEM_HPP

#include <string>
#include <anax/System.hpp>
#include "components/render.hpp"

namespace systems{

struct RenderSystem : anax::System<anax::Requires<components::TextureComponent>>
{
    RenderSystem(std::string title, int posx, int posy, int width, int height, Uint32 windowflags, Uint32 renderflags);
    ~RenderSystem();

    void render();

    bool isValid() const;
    sdl::Renderer *getRenderer() const;
    sdl::Window *getWindow() const;

private:
    sdl::Window *pwindow;
    sdl::Renderer *prenderer;

};

}
#endif /* SYSTEMS_RENDERSYSTEM_HPP */
