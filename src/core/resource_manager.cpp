#include "resource_manager.hpp"

#include <iostream>

namespace core
{

ResourceManagerTexture::ResourceManagerTexture()
{
    m_p_default_renderer = nullptr;
}
ResourceManagerTexture::~ResourceManagerTexture()
{
}

std::shared_ptr<sdl_wrap::Texture> ResourceManagerTexture::get(
    std::string texture_path, sdl_wrap::Renderer* p_renderer)
{
    auto search = m_loaded_textures.find(texture_path);
    std::shared_ptr<sdl_wrap::Texture> p_texture = nullptr;
    if (search != m_loaded_textures.end())
    {
        std::cout << "Already loaded texture " << texture_path << std::endl;
        p_texture = search->second;
    }
    else
    {
        std::cout << "Loading new texture " << texture_path << std::endl;
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

    std::cout << "Unloading textures " << std::endl;
    auto it = m_loaded_textures.begin();
    while (it != m_loaded_textures.end())
    {
        if (it->second.use_count() <= 1)
        {
            std::cout << "Unloading texture " << it->first << std::endl;
            it = m_loaded_textures.erase(it);
        }
        else
        {
            std::cout << "Not unloading texture " << it->first << " still used "
                      << it->second.use_count() << " times" << std::endl;
            it++;
        }
    }
}

} // namespace core
