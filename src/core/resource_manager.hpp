#ifndef CORE_RESOURCEMANAGER_HPP
#define CORE_RESOURCEMANAGER_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <sdl_wrap/wrap.hpp>
namespace core
{

class ResourceManagerTexture
{
public:
    ResourceManagerTexture();
    ~ResourceManagerTexture();

    std::shared_ptr<sdl_wrap::Texture> get(
        const std::string texture_path, sdl_wrap::Renderer* p_renderer);

    std::shared_ptr<sdl_wrap::Texture> get(const std::string texture_path);

    void set_default_renderer(sdl_wrap::Renderer* p_renderer);

    bool is_loaded(const std::string texture_path) const;

    void unload_unused();

private:
    std::unordered_map<std::string, std::shared_ptr<sdl_wrap::Texture>>
        m_loaded_textures;

    sdl_wrap::Renderer* m_p_default_renderer;
};
}
#endif /* CORE_RESOURCEMANAGER_HPP */
