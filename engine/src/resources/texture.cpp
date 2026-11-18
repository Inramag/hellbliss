#include <hellbliss/resources/texture.hpp>

#include <stdexcept>

namespace hellbliss::resources {
  Texture::Texture(int width, int height, int channels, const unsigned char* pixels) : _width(width), _height(height) {
        if (width <= 0 || height <= 0 || !pixels) throw std::invalid_argument("Invalid texture data");

        GLenum format = 0;
        GLenum internal_format = 0;
        switch (channels) {
            case 1: format = GL_RED;  internal_format = GL_R8;    break;
            case 2: format = GL_RG;   internal_format = GL_RG8;   break;
            case 3: format = GL_RGB;  internal_format = GL_RGB8;  break;
            case 4: format = GL_RGBA; internal_format = GL_RGBA8; break;
            default: throw std::invalid_argument("Unsupported texture channels");
        }

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        GLint previous_alignment = 4;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_alignment);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

        glPixelStorei(GL_UNPACK_ALIGNMENT, previous_alignment);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture() {
        if (_id) glDeleteTextures(1, &_id);
    }

    Texture::Texture(Texture&& other) noexcept : _id(other._id), _width(other._width), _height(other._height) {
        other._id = 0;
        other._width = 0;
        other._height = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this == &other) return *this;
        if (_id) glDeleteTextures(1, &_id);

        _id = other._id;
        _width = other._width;
        _height = other._height;

        other._id = 0;
        other._width = 0;
        other._height = 0;

        return *this;
    }
}
