#include "sdl_wrap/wrap.hpp"

namespace sdl_wrap
{

std::shared_ptr<spdlog::logger> sp_logger;

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
    sp_logger = common::logging_get_logger("sdlwrap");

    sp_logger->info("Initializing SDL");
    auto result = SDL_Init(sdl_flags);
    if (result){
        sdl_throw_error("SDL_Init");
    }

    sp_logger->info("Initializing SDL image");
    result = IMG_Init(img_flags);
    if (result != img_flags){
        sdl_img_throw_error("IMG_Init");
    }
}


Window::Window(
    const std::string title, int x, int y, int w, int h, Uint32 flags)
{
    sp_logger->info("Constructing window '{}'", title);

    m_p_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

    if (m_p_window == nullptr)
    {
        sdl_throw_error("SDL_CreateWindow: ");
    }
}

Window::~Window()
{
    sp_logger->info("Destroying window");

    if (m_p_window != nullptr)
    {
        SDL_DestroyWindow(m_p_window);
    }else{
        sp_logger->error("Window pointer is null!");
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
    sp_logger->info("Constructing renderer");

    m_p_renderer = SDL_CreateRenderer(window.get_pointer(), index, flags);
    if (m_p_renderer == nullptr)
    {
        sdl_throw_error(std::string("SDL_CreateRenderer"));
    }
}

Renderer::~Renderer()
{
    sp_logger->info("Destroying renderer");

    if (m_p_renderer == nullptr)
    {
        SDL_DestroyRenderer(m_p_renderer);
    }else{
        sp_logger->error("Renderer pointer is null!");
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

    sp_logger->info("Loading texture from path '{}'", path);

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
