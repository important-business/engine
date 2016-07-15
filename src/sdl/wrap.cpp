#include "sdl/wrap.hpp"
#include <sstream>

using namespace sdl;

Window::Window(
    const std::string title, int x, int y, int w, int h, Uint32 flags)
{
    window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

    if (window == nullptr)
    {
        std::ostringstream error;
        error << "Error:SDL_CreateWindow: " << SDL_GetError() << "\n";
        throw sdlexception(error.str());
    }
}

Window::~Window()
{
    std::cout << "Entering window destructor\n";
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
    }
}

SDL_Window* Window::get_pointer()
{
    return window;
}

Renderer::Renderer(Window& window, int index, Uint32 flags)
{
    prenderer = SDL_CreateRenderer(window.get_pointer(), index, flags);
    if (prenderer == nullptr)
    {
        std::ostringstream error;
        error << "Error:Renderer: " << SDL_GetError() << "\n";
        throw sdlexception(error.str());
    }
}

Renderer::~Renderer()
{
    std::cout << "Entering renderer destructor\n";
    if (prenderer == nullptr)
    {
        SDL_DestroyRenderer(prenderer);
    }
}

int Renderer::clear()
{
    return SDL_RenderClear(prenderer);
}

int Renderer::copy(Texture& texture,
    const SDL_Rect* srcrect,
    const SDL_Rect* dstrect,
    int angle,
    SDL_RendererFlip flip)
{
    return SDL_RenderCopyEx(prenderer,
        texture.get_pointer(),
        srcrect,
        dstrect,
        angle,
        nullptr,
        flip);
}

void Renderer::present()
{
    SDL_RenderPresent(prenderer);
}

int Renderer::set_draw_color(Uint8 red, Uint8 blue, Uint8 green, Uint8 alpha)
{
    return SDL_SetRenderDrawColor(prenderer, red, blue, green, alpha);
}

SDL_Renderer* Renderer::get_pointer()
{
    return prenderer;
}

Texture::Texture(std::string path, Renderer& renderer)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        std::ostringstream error;
        error << "Error:Texture: " << IMG_GetError() << "\n";
        throw sdlexception(error.str());
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderer.get_pointer(), surface);
    if (texture == nullptr)
    {
        std::ostringstream error;
        error << "Error:Texture: " << SDL_GetError() << "\n";
        throw sdlexception(error.str());
    }

    this->mWidth = surface->w;
    this->mHeight = surface->h;

    SDL_FreeSurface(surface);

    this->mTexture = texture;
}

SDL_Texture* Texture::get_pointer()
{
    return mTexture;
}

const char* sdlexception::what() const throw()
{
    return exceptionstring.c_str();
}
