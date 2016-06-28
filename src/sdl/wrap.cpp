#include "sdl/wrap.hpp"
#include <sstream>

using namespace sdl;

Window::Window(
    const std::string title,
    int         x,
    int         y,
    int         w,
    int         h,
    Uint32      flags
)
{
    window = SDL_CreateWindow(
        title.c_str(),
        x,
        y,
        w,
        h,
        flags
    );
    if (window == nullptr)
    {
        std::ostringstream error;
        error << "Error:SDL_CreateWindow: " << SDL_GetError() << "\n";
        auto error_str = error.str();
        throw error_str;
    }
}

Window::~Window()
{
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
    }
}

SDL_Window* Window::get_pointer()
{
    return window;
}


Texture::Texture(std::string path)
{
    SDL_Renderer* render = NULL;

    SDL_Surface* surface = IMG_Load( path.c_str() );
    if (surface == nullptr)
    {
        std::ostringstream error;
        error << "Error:Texture: " << IMG_GetError() << "\n";
        auto error_str = error.str();
        throw error_str;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    if ( texture == nullptr)
    {
        std::ostringstream error;
        error << "Error:Texture: " << SDL_GetError() << "\n";
        auto error_str = error.str();
        throw error_str;
    }

    this->mWidth = surface->w;
    this->mHeight = surface->h;

    SDL_FreeSurface(surface);

    this->mTexture = texture;
}
