/**
 * @file
 * @brief Core Resource Manager Header
 *
 * Contains definition of classes for managing resources
 */

#ifndef CORE_RESOURCE_MANAGER_HPP
#define CORE_RESOURCE_MANAGER_HPP

#include "common/logging.hpp"

#include <memory>
#include <sdl_wrap/wrap.hpp>
#include <string>
#include <unordered_map>

namespace core
{

class ResourceManagerTexture
{
public:
    ResourceManagerTexture();
    ~ResourceManagerTexture() = default;

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

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}
#endif /* CORE_RESOURCE_MANAGER_HPP */
