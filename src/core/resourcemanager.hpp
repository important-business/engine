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

    void set_default_renderer(sdl::Renderer* prenderer);

    bool is_loaded(const std::string texturepath) const;

    void unload_unused(void);

private:
    std::unordered_map<std::string, std::shared_ptr<sdl::Texture>>
        m_loaded_textures;

    sdl::Renderer* m_p_default_renderer;
};
}
#endif /* CORE_RESOURCEMANAGER_HPP */
