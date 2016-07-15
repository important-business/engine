#ifndef CORE_RESOURCEMANAGER_HPP
#define CORE_RESOURCEMANAGER_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <sdl/wrap.hpp>
namespace core
{

class ResourceManagerTexture
{
public:
    ResourceManagerTexture();
    ~ResourceManagerTexture();

    std::shared_ptr<sdl::Texture> get(
        const std::string texturepath, sdl::Renderer* prenderer);
    std::shared_ptr<sdl::Texture> get(const std::string texturepath);
    void setDefaultRenderer(sdl::Renderer* prenderer);

    bool isLoaded(const std::string texturepath) const;

    void unloadUnused(void);

private:
    std::unordered_map<std::string, std::shared_ptr<sdl::Texture>>
        loadedtextures;
    sdl::Renderer* defaultrenderer;
};
}
#endif /* CORE_RESOURCEMANAGER_HPP */
