#pragma once

#include <hellbliss/resources/texture.hpp>
#include <hellbliss/resources/shader.hpp>

#include <filesystem>
#include <unordered_map>
#include <string>
#include <memory>

namespace hellbliss {
  class Resources {
  public:
    Resources(std::filesystem::path resources);
    ~Resources() = default;

    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;

    Resources(Resources&&) = default;
    Resources& operator=(Resources&&) = default;

    resources::Texture* load_texture(const std::string& id, const std::string& path);
    resources::Texture* get_texture(const std::string& id);

    resources::Shader* load_shader(const std::string& id, const std::string& name);
    resources::Shader* get_shader(const std::string& id);

  private:
    std::filesystem::path resources_;

    std::unordered_map<std::string, std::unique_ptr<resources::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<resources::Shader>> shaders;
  };
}
