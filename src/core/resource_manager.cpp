#include "resource_manager.hpp"

#include <iostream>

namespace core
{

ResourceManagerTexture::ResourceManagerTexture()
{
    m_p_default_renderer = nullptr;
    m_sp_logger = logging_get_logger("resource_manager");
}

std::shared_ptr<sdl_wrap::Texture> ResourceManagerTexture::get(
    std::string texture_path, sdl_wrap::Renderer* p_renderer)
{
    auto search = m_loaded_textures.find(texture_path);
    std::shared_ptr<sdl_wrap::Texture> p_texture = nullptr;
    if (search != m_loaded_textures.end())
    {
        m_sp_logger->info("Already loaded texture {}", texture_path);
        p_texture = search->second;
    }
    else
    {
        m_sp_logger->info("Loading new texture {}", texture_path);
        p_texture =
            std::make_shared<sdl_wrap::Texture>(texture_path, *p_renderer);
        // Do some checking of p_texture here
        m_loaded_textures[texture_path] = p_texture;
    }
    return p_texture;
}

std::shared_ptr<sdl_wrap::Texture> ResourceManagerTexture::get(
    const std::string texture_path)
{
    return get(texture_path, m_p_default_renderer);
}

bool ResourceManagerTexture::is_loaded(const std::string texture_path) const
{
    auto search = m_loaded_textures.find(texture_path);
    return (search != m_loaded_textures.end());
}
void ResourceManagerTexture::set_default_renderer(
    sdl_wrap::Renderer* p_renderer)
{
    m_p_default_renderer = p_renderer;
}

void ResourceManagerTexture::unload_unused()
{

    m_sp_logger->info("Unloading unused textures");
    auto it = m_loaded_textures.begin();
    while (it != m_loaded_textures.end())
    {
        if (it->second.use_count() <= 1)
        {
            m_sp_logger->info("Unloading unused texture {}", it->first);
            it = m_loaded_textures.erase(it);
        }
        else
        {
            m_sp_logger->info(
                "Not unloading unused texture {}, still used {} times",
                it->first,
                it->second.use_count());
            it++;
        }
    }
}

} // namespace core
