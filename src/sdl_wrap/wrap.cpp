#include "sdl_wrap/wrap.hpp"

#include <iostream>
#include <sstream>

namespace sdl_wrap
{

Window::Window(
    const std::string title, int x, int y, int w, int h, Uint32 flags)
{
    m_p_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

    if (m_p_window == nullptr)
    {
        std::ostringstream error;
        error << "Error:SDL_CreateWindow: " << SDL_GetError() << "\n";
        throw Exception(error.str());
    }
}

Window::~Window()
{
    std::cout << "Entering window destructor\n";
    if (m_p_window != nullptr)
    {
        SDL_DestroyWindow(m_p_window);
    }
}

SDL_Window* Window::get_pointer()
{
    return m_p_window;
}

void Window::get_size(int* p_w, int* p_h)
{
    SDL_GetWindowSize(m_p_window, p_w, p_h);
}

Renderer::Renderer(Window& window, int index, Uint32 flags)
{
    m_p_renderer = SDL_CreateRenderer(window.get_pointer(), index, flags);
    if (m_p_renderer == nullptr)
    {
        std::ostringstream error;
        error << "Error:Renderer: " << SDL_GetError() << "\n";
        throw Exception(error.str());
    }
}

Renderer::~Renderer()
{
    std::cout << "Entering renderer destructor\n";
    if (m_p_renderer == nullptr)
    {
        SDL_DestroyRenderer(m_p_renderer);
    }
}

int Renderer::clear()
{
    return SDL_RenderClear(m_p_renderer);
}

int Renderer::copy(Texture& texture,
    const SDL_Rect* src_rect,
    const SDL_Rect* dest_rect,
    int angle,
    SDL_RendererFlip flip)
{
    return SDL_RenderCopyEx(m_p_renderer,
        texture.get_pointer(),
        src_rect,
        dest_rect,
        angle,
        nullptr,
        flip);
}

int Renderer::fill_rect(const SDL_Rect* p_rect)
{
    return SDL_RenderFillRect(m_p_renderer, p_rect);
}

void Renderer::present()
{
    SDL_RenderPresent(m_p_renderer);
}

int Renderer::set_draw_color(Uint8 red, Uint8 blue, Uint8 green, Uint8 alpha)
{
    return SDL_SetRenderDrawColor(m_p_renderer, red, blue, green, alpha);
}

SDL_Renderer* Renderer::get_pointer()
{
    return m_p_renderer;
}

Texture::Texture(std::string path, Renderer& renderer)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        std::ostringstream error;
        error << "Error:Texture: " << IMG_GetError() << "\n";
        throw Exception(error.str());
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderer.get_pointer(), surface);
    if (texture == nullptr)
    {
        std::ostringstream error;
        error << "Error:Texture: " << SDL_GetError() << "\n";
        throw Exception(error.str());
    }

    m_width = surface->w;
    m_height = surface->h;

    SDL_FreeSurface(surface);

    m_p_texture = texture;
}

int Texture::get_width()
{
    return m_width;
}

int Texture::get_height()
{
    return m_height;
}

SDL_Texture* Texture::get_pointer()
{
    return m_p_texture;
}

const char* Exception::what() const throw()
{
    return m_message.c_str();
}

} // namespace sdl
