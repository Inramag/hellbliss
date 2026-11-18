#pragma once

#include <hellbliss/script.hpp>

#include <hellbliss/color.h>
#include <hellbliss/resources/texture.hpp>
#include <hellbliss/resources/shader.hpp>

namespace hellbliss::scripts {
  class Sprite : public Script {
  public:
    enum class TextureMode {
      stretch, fit
    };
    
    Color color{};
    resources::Texture* texture = nullptr;
    TextureMode mode = TextureMode::stretch;

    resources::Shader* shader = nullptr;

  private:
    void render() override;
  };
}
