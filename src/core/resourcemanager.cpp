#include "resourcemanager.hpp"

#include <iostream>

namespace core
{

ResourceManagerTexture::ResourceManagerTexture()
{
    defaultrenderer = nullptr;
}
ResourceManagerTexture::~ResourceManagerTexture()
{
}

std::shared_ptr<sdl::Texture> ResourceManagerTexture::get(
    std::string texturepath, sdl::Renderer* prenderer)
{
    auto search = loadedtextures.find(texturepath);
    if (search != loadedtextures.end())
    {
        std::cout << "Already loaded texture " << texturepath << std::endl;
        return search->second;
    }
    else
    {
        std::cout << "Loading new texture " << texturepath << std::endl;
        auto ptexture = std::make_shared<sdl::Texture>(texturepath, *prenderer);
        // Do some checking of ptexture here
        loadedtextures[texturepath] = ptexture;
        return ptexture;
    }
}

std::shared_ptr<sdl::Texture> ResourceManagerTexture::get(
    const std::string texturepath)
{
    return get(texturepath, defaultrenderer);
}

bool ResourceManagerTexture::isLoaded(const std::string texturepath) const
{
    auto search = loadedtextures.find(texturepath);
    if (search != loadedtextures.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}
void ResourceManagerTexture::setDefaultRenderer(sdl::Renderer* prenderer)
{
    defaultrenderer = prenderer;
}

void ResourceManagerTexture::unloadUnused(void)
{

    std::cout << "Unloading textures " << std::endl;
    auto it = loadedtextures.begin();
    while (it != loadedtextures.end())
    {
        if (it->second.use_count() <= 1)
        {
            std::cout << "Unloading texture " << it->first << std::endl;
            it = loadedtextures.erase(it);
        }
        else
        {
            std::cout << "Not unloading texture " << it->first << " still used "
                      << it->second.use_count() << " times" << std::endl;
            it++;
        }
    }
}
}
