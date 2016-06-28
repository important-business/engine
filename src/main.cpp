#include <iostream>
#include <sstream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "core/example_module.hpp"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "Engine";

namespace sdl {
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

            ~Window()
            {
                if (window != nullptr)
                {
                    SDL_DestroyWindow(window);
                }
            }

            SDL_Window* get_pointer()
            {
                return window;
            }
    };


class Texture
{
    private:
        SDL_Texture* mTexture = NULL;
        int mWidth = 0;
        int mHeight = 0;

    public:
        Texture(std::string path)
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
};
}

bool handle_input();


int main(int argc, char* argv[])
{
    std::cout << "Yay, you built code!\n";
    auto a = core::ExampleModule(4);
    auto i = a.getme();
    std::cout << "Returned value " << i << "\n";

    auto window = sdl::Window(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    auto pWindow = window.get_pointer();
    // SDL_Surface* surface = SDL_GetWindowSurface(pWindow);

    auto is_not_done = true;
    while (is_not_done)
    {
        is_not_done = handle_input();
        SDL_UpdateWindowSurface(pWindow);
    }
}


bool handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
        else
        {
        }
    }
    return true;
}
