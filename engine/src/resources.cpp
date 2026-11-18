#include <hellbliss/resources.hpp>

#include <hellbliss/core/logger.hpp>

#include <filesystem>
#include <memory>

#include <stb/stb_image.h>

namespace hellbliss {
  Resources::Resources(std::filesystem::path resources) {
    resources_ = resources;
  }
  
  resources::Texture* Resources::load_texture(const std::string& id, const std::string& name) {
    const std::filesystem::path path = resources_ / "textures" / name;
    core::Logger::log("resources.load.texture: " + path.string() + " ");

    if (textures.find(id) != textures.end()) {
      core::Logger::log("\033[31mERROR\033[0m\n");
      core::Logger::log("  Texture with id '" + id + "' already exists\n");
      return nullptr;
    }

    try {
      int w = 0, h = 0, c = 0;
      std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> pixels(stbi_load(path.c_str(), &w, &h, &c, 0), stbi_image_free);
      if (!pixels) {
        core::Logger::log("\033[31mERROR\033[0m\n  " + std::string(stbi_failure_reason()) + "\n");
        return nullptr;
      }

      auto texture = std::make_unique<resources::Texture>(w, h, c, pixels.get());
      auto* result = texture.get();
    
      textures.emplace(id, std::move(texture));

      core::Logger::log("\033[32mSUCCESS\033[0m\n");
      return result;
    } catch (const std::exception& e) {
      core::Logger::log("\033[31mERROR\033[0m\n  " + std::string(e.what()) + "\n");
      return nullptr;
    }
  }

  resources::Texture* Resources::get_texture(const std::string& id) {
    auto it = textures.find(id);
    if (it == textures.end()) return nullptr;
    return it->second.get();
  }

  resources::Shader* Resources::load_shader(const std::string& id, const std::string& name) {
    const std::filesystem::path path = resources_ / "shaders" / name;
    core::Logger::log("resources.load.shader: " + path.string() + " ");

    if (shaders.find(id) != shaders.end()) {
      core::Logger::log("\033[31mERROR\033[0m\n");
      core::Logger::log("  Shader with id '" + id + "' already exists\n");
      return nullptr;
    }

    try {
      auto shader = std::make_unique<resources::Shader>(path.string() + ".vsh", path.string() + ".fsh");

      resources::Shader* res = shader.get();
      shaders.emplace(id, std::move(shader));

      core::Logger::log("\033[32mSUCCESS\033[0m\n");
      return res;
    } catch (const std::exception& e) {
      core::Logger::log("\033[31mERROR\033[0m\n  " + std::string(e.what()) + "\n");
      return nullptr;
    }
  }

    resources::Shader* Resources::get_shader(const std::string& id) {
        auto it = shaders.find(id);
        if (it == shaders.end()) return nullptr;
        return it->second.get();
    }
}
