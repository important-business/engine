#ifndef CORE_RESOURCEMANAGER_HPP
#define CORE_RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <sdl/wrap.hpp>
namespace core
{

class ResourceManagerTexture
{
public:
    ResourceManagerTexture();
    ~ResourceManagerTexture();

    sdl::Texture* get(const std::string texturepath, sdl::Renderer* prenderer);
    sdl::Texture* get(const std::string texturepath);
    void setDefaultRenderer(sdl::Renderer* prenderer);
    void unload(const std::string texturepath);
    void unload(sdl::Texture*);

    bool isLoaded(const std::string texturepath) const;

private:
    std::unordered_map<std::string, sdl::Texture*> loadedtextures;
    sdl::Renderer* defaultrenderer;
};
}
#endif /* CORE_RESOURCEMANAGER_HPP */
