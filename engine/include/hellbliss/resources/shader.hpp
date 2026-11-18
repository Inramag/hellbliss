#pragma once

#include <glad/gl.h>

#include <string>
#include <unordered_map>

namespace hellbliss::resources {
  class Shader {
  public:
    using UpdateCallback = void(*)(const Shader*);

    Shader(const std::string& vertex_path, const std::string& fragment_path);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    GLuint id() const { return _id; }

    void set_float(const std::string& name, float value);
    void set_int(const std::string& name, int value);
    void set_vec2(const std::string& name, float x, float y);
    void set_vec3(const std::string& name, float x, float y, float z);
    void set_vec4(const std::string& name, float x, float y, float z, float w);

  private:
    GLint get_uloc(const std::string& name);
    
    GLuint _id = 0;
    std::unordered_map<std::string, GLint> _ulocs;
  };
}
