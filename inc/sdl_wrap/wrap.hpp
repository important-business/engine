#ifndef SDL_WRAP_HPP
#define SDL_WRAP_HPP

#include <exception>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

namespace sdl_wrap
{

class Texture;

class Window
{
public:
    Window(const std::string title, int x, int y, int w, int h, Uint32 flags);
    Window(const Window&) = delete;
    ~Window();

    SDL_Window* get_pointer();

    void get_size(int* p_w, int* p_h);

private:
    SDL_Window* m_p_window;
};

class Renderer
{
public:
    Renderer(Window& window, int index, Uint32 flags);
    Renderer(const Renderer&) = delete;
    ~Renderer();

    int clear();

    int copy(Texture& texture,
        const SDL_Rect* src_rect,
        const SDL_Rect* dest_rect,
        int angle,
        SDL_RendererFlip flip);

    int fill_rect(const SDL_Rect* p_rect);

    void present();

    int set_draw_color(Uint8 red, Uint8 blue, Uint8 green, Uint8 alpha);

    SDL_Renderer* get_pointer();

private:
    SDL_Renderer* m_p_renderer;
};

class Texture
{
public:
    Texture(std::string path, Renderer& renderer);
    Texture(const Texture&) = delete;

    SDL_Texture* get_pointer();

    int get_width();

    int get_height();

private:
    SDL_Texture* m_p_texture = NULL;

    int m_width{0};

    int m_height{0};
};

class Exception : public std::exception
{
public:
    Exception(std::string message) : m_message(message)
    {
    }

    virtual const char* what() const throw();

private:
    std::string m_message;
};
}
#endif /* SDL_WRAP_HPP */
