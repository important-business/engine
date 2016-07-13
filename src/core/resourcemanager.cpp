#include "resourcemanager.hpp"

#include <iostream>

using namespace core;

ResourceManagerTexture::ResourceManagerTexture()
{
    defaultrenderer = nullptr;
}
ResourceManagerTexture::~ResourceManagerTexture()
{
}

sdl::Texture* ResourceManagerTexture::get(
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
        std::cout << "Loading new texture" << texturepath << std::endl;
        auto ptexture = new sdl::Texture(texturepath, *prenderer);
        // Do some checking of ptexture here
        loadedtextures[texturepath] = ptexture;
        return ptexture;
    }
}

sdl::Texture* ResourceManagerTexture::get(const std::string texturepath)
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
void ResourceManagerTexture::unload(const std::string texturepath)
{
}

void ResourceManagerTexture::unload(sdl::Texture*)
{
}
