#pragma once

#include <hellbliss/resources/shader.hpp>
#include <hellbliss/resources/texture.hpp>

#include <hellbliss/color.h>
#include <hellbliss/vec2i.h>

#include <vector>

namespace hellbliss::render {
  class Renderer {
  public:
    Renderer() = default;
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void initialize(const resources::Shader& shader);
    
    void draw_rect(vec2i pos, vec2i size, Color color, const resources::Shader* shader = nullptr);
    void draw_texture(vec2i pos, vec2i size, const resources::Texture& tex, Color color, const resources::Shader* shader = nullptr);

    void flush();

  private:
    struct Vertex {
      float x, y;
      float r, g, b, a;
      float u, v;
    };

    static constexpr std::size_t max_rects = 2000;
    static constexpr std::size_t vertices_per_rect = 4;
    static constexpr std::size_t indices_per_rect = 6;

    void prepare_batch(const resources::Shader& shader, GLuint texture, bool textured);

    bool is_shader_valid(const resources::Shader* shader) const;
    const resources::Shader* resolve_shader(const resources::Shader* requested) const;

    const resources::Shader* def_shader_ = nullptr;

    GLuint vao_ = 0, vbo_ = 0, ebo_ = 0;

    GLint viewport_loc_ = -1;
    GLint tex_loc_ = -1;
    GLint use_tex_loc_ = -1;

    const resources::Shader* batch_shader_ = nullptr;
    GLuint batch_tex_ = 0;
    bool batch_texd_ = false;

    std::vector<Vertex> vertices_;

    bool initialized_ = false;
  };
}
