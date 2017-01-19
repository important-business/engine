#include "sdl_wrap/wrap.hpp"

#include <iostream>
#include <sstream>

namespace sdl_wrap
{

inline void sdl_throw_error(const std::string &functionname){
        auto error = std::string("Error-") + functionname;
        error += ": ";
        error+= SDL_GetError();
        error += '\n';
        throw SdlException(error);
}

inline void sdl_img_throw_error(const std::string &functionname){
        auto error = std::string("Error-") + functionname;
        error += ": ";
        error+= IMG_GetError();
        error += '\n';
        throw SdlException(error);
}

void sdl_init(uint32_t sdl_flags, int img_flags)
{
    auto result = SDL_Init(sdl_flags);
    if (result){
        sdl_throw_error("SDL_Init");
    }

    result = IMG_Init(img_flags);
    if (result != img_flags){
        sdl_img_throw_error("IMG_Init");
    }
}


Window::Window(
    const std::string title, int x, int y, int w, int h, Uint32 flags)
{
    m_p_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

    if (m_p_window == nullptr)
    {
        sdl_throw_error("SDL_CreateWindow: ");
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
        sdl_throw_error(std::string("SDL_CreateRenderer"));
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

void Renderer::clear()
{
    auto result = SDL_RenderClear(m_p_renderer);
    if (result)
    {
        sdl_throw_error(std::string("SDL_RenderClear"));
    }
}

void Renderer::copy(Texture& texture,
    const SDL_Rect* src_rect,
    const SDL_Rect* dest_rect,
    int angle,
    SDL_RendererFlip flip)
{
    auto result = SDL_RenderCopyEx(m_p_renderer,
        texture.get_pointer(),
        src_rect,
        dest_rect,
        angle,
        nullptr,
        flip);
    if (result)
    {
        sdl_throw_error(std::string("SDL_RenderCopyEx"));
    }
}

void Renderer::fill_rect(const SDL_Rect* p_rect)
{
    auto result = SDL_RenderFillRect(m_p_renderer, p_rect);
    if (result)
    {
        sdl_throw_error(std::string("SDL_RenderFillRect"));
    }
}

void Renderer::present()
{
    SDL_RenderPresent(m_p_renderer);
}

void Renderer::set_draw_color(Uint8 red, Uint8 blue, Uint8 green, Uint8 alpha)
{
    auto result = SDL_SetRenderDrawColor(m_p_renderer, red, blue, green, alpha);
    if (result)
    {
        sdl_throw_error(std::string("SDL_SetRenderDrawColor"));
    }
}

void Renderer::set_draw_blend_mode(SDL_BlendMode blendmode)
{
    auto result = SDL_SetRenderDrawBlendMode(m_p_renderer, blendmode);
    if (result)
    {
        sdl_throw_error(std::string("SDL_SetRenderDrawBlendMode"));
    }
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
        sdl_throw_error(std::string("IMG_Load"));
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderer.get_pointer(), surface);
    if (texture == nullptr)
    {
        sdl_throw_error(std::string("SDL_CreateTextureFromSurface"));
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

} // namespace sdl
