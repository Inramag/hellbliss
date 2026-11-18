#pragma once

#include <glad/gl.h>

namespace hellbliss::resources {
  class Texture {
  public:
    Texture(int width, int height, int channels, const unsigned char* pixels);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    GLuint id() const { return _id; }
    int width() const { return _width; }
    int height() const { return _height; }

  private:
    GLuint _id = 0;

    int _width = 0;
    int _height = 0;
  };
}
