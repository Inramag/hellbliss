#include "hellbliss/resources/shader.hpp"
#include <hellbliss/render/renderer.hpp>

#include <hellbliss/resources.hpp>

#include <glad/gl.h>

namespace hellbliss::render {
  struct RendererVertex {
    GLfloat x, y;
    GLfloat r, g, b, a;
    GLfloat u, v;
  };

  void Renderer::initialize(const resources::Shader& shader) {
    if (initialized_) return;

    def_shader_ = &shader;

    const GLuint program = shader.id();

    viewport_loc_ = glGetUniformLocation(program, "viewport");
    tex_loc_ = glGetUniformLocation(program, "tex");
    use_tex_loc_ = glGetUniformLocation(program, "use_tex");

    vertices_.reserve(max_rects * vertices_per_rect);

    std::vector<GLuint> indices;
    indices.reserve(max_rects * indices_per_rect);

    for (std::size_t i = 0; i < max_rects; ++i) {
      const GLuint offset = static_cast<GLuint>(i * vertices_per_rect);

      indices.push_back(offset + 0);
      indices.push_back(offset + 1);
      indices.push_back(offset + 2);

      indices.push_back(offset + 2);
      indices.push_back(offset + 3);
      indices.push_back(offset + 0);
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(max_rects * vertices_per_rect * sizeof(RendererVertex)), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(RendererVertex), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(RendererVertex), reinterpret_cast<void*>(offsetof(RendererVertex, r)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RendererVertex), reinterpret_cast<void*>(offsetof(RendererVertex, u)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    initialized_ = true;
  }

  void Renderer::prepare_batch(const resources::Shader& shader, GLuint texture, bool textured) {
    const bool shader_changed = batch_shader_ != &shader;
     const bool texture_changed = batch_tex_ != texture || batch_texd_ != textured;
    
    if (!vertices_.empty() && (shader_changed || texture_changed)) flush();

    batch_shader_ = &shader;
    batch_tex_ = texture;
    batch_texd_ = textured;
  }

  bool Renderer::is_shader_valid(const resources::Shader* shader) const {
    if (!shader) return false;

    const GLuint program = shader->id();
    if (program == 0 || glIsProgram(program) != GL_TRUE) return false;

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    return linked == GL_TRUE;
  }

  const resources::Shader* Renderer::resolve_shader(const resources::Shader* requested) const {
    if (is_shader_valid(requested)) return requested;

    if (requested != def_shader_ &&
        is_shader_valid(def_shader_)) {
        return def_shader_;
    }

    return nullptr;
  }

  void Renderer::draw_rect(vec2i pos, vec2i size, Color color, const resources::Shader* req_shader) {
    if (size.x <= 0 || size.y <= 0) return;
    if (!initialized_) return;

    const auto* shader = resolve_shader(req_shader);
    if (!shader) return;

    prepare_batch(*shader, 0, false);

    if (vertices_.size() >= max_rects * vertices_per_rect) flush();

    const GLfloat x = static_cast<GLfloat>(pos.x);
    const GLfloat y = static_cast<GLfloat>(pos.y);

    const GLfloat width = static_cast<GLfloat>(size.x);
    const GLfloat height = static_cast<GLfloat>(size.y);

    const GLfloat r = static_cast<GLfloat>(color.r) / 255.0f;
    const GLfloat g = static_cast<GLfloat>(color.g) / 255.0f;
    const GLfloat b = static_cast<GLfloat>(color.b) / 255.0f;
    const GLfloat a = static_cast<GLfloat>(color.a) / 255.0f;

    vertices_.push_back({x, y, r, g, b, a, 0.0f, 0.0f});
    vertices_.push_back({x + width, y, r, g, b, a, 1.0f, 0.0f});
    vertices_.push_back({x + width, y + height, r, g, b, a, 1.0f, 1.0f});
    vertices_.push_back({x, y + height, r, g, b, a, 0.0f, 1.0f});
  }

  void Renderer::draw_texture(vec2i pos, vec2i size, const resources::Texture& texture, Color color, const resources::Shader* req_shader) {
    if (size.x <= 0 || size.y <= 0) return;
    if (texture.id() == 0) return;
    if (!initialized_) return;

    const auto* shader = resolve_shader(req_shader);
    if (!shader) return;

    prepare_batch(*shader, texture.id(), true);

    if (vertices_.size() >= max_rects * vertices_per_rect) {
        flush();
    }

    const GLfloat x = static_cast<GLfloat>(pos.x);
    const GLfloat y = static_cast<GLfloat>(pos.y);

    const GLfloat width = static_cast<GLfloat>(size.x);
    const GLfloat height = static_cast<GLfloat>(size.y);

    const GLfloat r = static_cast<GLfloat>(color.r) / 255.0f;
    const GLfloat g = static_cast<GLfloat>(color.g) / 255.0f;
    const GLfloat b = static_cast<GLfloat>(color.b) / 255.0f;
    const GLfloat a = static_cast<GLfloat>(color.a) / 255.0f;

    vertices_.push_back({x, y, r, g, b, a, 0.0f, 0.0f});
    vertices_.push_back({x + width, y, r, g, b, a, 1.0f, 0.0f});
    vertices_.push_back({x + width, y + height, r, g, b, a, 1.0f, 1.0f});
    vertices_.push_back({x, y + height, r, g, b, a, 0.0f, 1.0f});
  }

  void Renderer::flush() {
    if (!initialized_ || vertices_.empty()) return;

    const auto* shader = resolve_shader(batch_shader_);
    if (!shader) {
      vertices_.clear();
      batch_shader_ = nullptr;
      return;
    }

    batch_shader_ = shader;
    const GLuint program = shader->id();

    viewport_loc_ = glGetUniformLocation(program, "viewport");
    tex_loc_ = glGetUniformLocation(program, "tex");
    use_tex_loc_ = glGetUniformLocation(program, "use_tex");

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (viewport[2] <= 0 || viewport[3] <= 0) {
      vertices_.clear();
      return;
    }

    glUseProgram(program);

    glUniform2f(viewport_loc_, static_cast<GLfloat>(viewport[2]), static_cast<GLfloat>(viewport[3]));

    glUniform1i(use_tex_loc_, batch_texd_ ? 1 : 0);

    glUniform1i(tex_loc_, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, batch_texd_ ? batch_tex_ : 0);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertices_.size() * sizeof(RendererVertex)), vertices_.data());

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices_.size() / vertices_per_rect * indices_per_rect), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    vertices_.clear();
  }
}
