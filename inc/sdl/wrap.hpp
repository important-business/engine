#ifndef __SDL_WRAP_HPP
#define __SDL_WRAP_HPP

#include <exception>
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
    Window(const std::string title, int x, int y, int w, int h, Uint32 flags);

    Window(const Window&) = delete;

    ~Window();
    SDL_Window* get_pointer();
};

class Renderer
{
private:
    SDL_Renderer* prenderer;

public:
    Renderer(Window& window, int index, Uint32 flags);

    Renderer(const Renderer&) = delete;

    ~Renderer();

    int clear();

    int copy(Texture& texture,
        const SDL_Rect* srcrect,
        const SDL_Rect* dstrect,
        int angle,
        SDL_RendererFlip flip);

    void present();

    int set_draw_color(Uint8 red, Uint8 blue, Uint8 green, Uint8 alpha);

    SDL_Renderer* get_pointer();
};

class Texture
{
private:
    SDL_Texture* mTexture = NULL;
    int mWidth{0};
    int mHeight = {0};

public:
    Texture(std::string path, Renderer& renderer);

    Texture(const Texture&) = delete;

    SDL_Texture* get_pointer();
};

class sdlexception : public std::exception
{
public:
    sdlexception(std::string exceptionstring) : exceptionstring(exceptionstring)
    {
    }

    virtual const char* what() const throw();

private:
    std::string exceptionstring;
};
}
#endif
