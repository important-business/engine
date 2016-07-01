#ifndef __SDL_WRAP_HPP
#define __SDL_WRAP_HPP

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

namespace sdl
{

class Texture;

class Window
{
    private:
        SDL_Window* window;

    public:
        Window(
            const std::string title,
            int         x,
            int         y,
            int         w,
            int         h,
            Uint32      flags
        );
        ~Window();
        SDL_Window* get_pointer();
};

class Renderer
{
    private:
        SDL_Renderer* prenderer;

    public:
        Renderer(Window *pwindow, int index, Uint32 flags);
        ~Renderer();
        int clear();
        int copy(
                Texture *ptexture,
                const SDL_Rect* srcrect,
                const SDL_Rect* dstrect
                );
        void present();

        int set_draw_color(
                Uint8 red,
                Uint8 blue,
                Uint8 green,
                Uint8 alpha
                );

        SDL_Renderer* get_pointer();
};

class Texture
{
    private:
        SDL_Texture* mTexture = NULL;
        int mWidth = 0;
        int mHeight = 0;

    public:
        Texture(std::string path, Renderer *prender);
        SDL_Texture* get_pointer();
};
}
#endif
