#include <hellbliss/resources/shader.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace hellbliss::resources {
  namespace {
    std::string read_file(const std::string& path) {
      std::ifstream file(path);
      if (!file) throw std::runtime_error("Failed to open shader file: " + path);

      std::stringstream buffer;
      buffer << file.rdbuf();

      return buffer.str();
    }

    GLuint compile_shader(GLenum type, const std::string& source) {
      GLuint shader = glCreateShader(type);

      const char* src = source.c_str();
      glShaderSource(shader, 1, &src, nullptr);
      glCompileShader(shader);

      GLint success;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

      if (!success) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, '\0');
        glGetShaderInfoLog(shader, length, nullptr, log.data());

        glDeleteShader(shader);

        throw std::runtime_error("Shader compilation failed:\n" + log);
      }

      return shader;
    }
  }

  Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {
    GLuint vertex = compile_shader(GL_VERTEX_SHADER, read_file(vertex_path));

    GLuint fragment = 0;
    try {
      fragment = compile_shader(GL_FRAGMENT_SHADER, read_file(fragment_path));
      _id = glCreateProgram();

      glAttachShader(_id, vertex);
      glAttachShader(_id, fragment);
      glLinkProgram(_id);

      GLint success;
      glGetProgramiv(_id, GL_LINK_STATUS, &success);

      if (!success) {
        GLint length;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, '\0');
        glGetProgramInfoLog(_id, length, nullptr, log.data());

        glDeleteProgram(_id);
        _id = 0;

        throw std::runtime_error("Shader linking failed:\n" + log);
      }
    }
    catch (...) {
      glDeleteShader(vertex);
      if (fragment) glDeleteShader(fragment);
      throw;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  Shader::~Shader() {
    if (_id) glDeleteProgram(_id);
  }

  Shader::Shader(Shader&& other) noexcept : _id(std::exchange(other._id, 0)), _ulocs(std::move(other._ulocs)) {}

  Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
      if (_id) glDeleteProgram(_id);
      
      _id = std::exchange(other._id, 0);
      _ulocs = std::move(other._ulocs);
    }

    return *this;
  }

  GLint Shader::get_uloc(const std::string& name) {
    auto it = _ulocs.find(name);
    if (it != _ulocs.end()) return it->second;

    GLint loc = glGetUniformLocation(_id, name.c_str());
    _ulocs.emplace(name, loc);

    return loc;
  }

  void Shader::set_float(const std::string& name, float value) {
    GLint loc = get_uloc(name);
    if (loc != -1) glUniform1f(loc, value);
  }
  void Shader::set_int(const std::string& name, int value) {
    GLint loc = get_uloc(name);
    if (loc != -1) glUniform1i(loc, value);
  }
  void Shader::set_vec2(const std::string& name, float x, float y) {
    GLint loc = get_uloc(name);
    if (loc != -1) glUniform2f(loc, x, y);
  }
  void Shader::set_vec3(const std::string& name, float x, float y, float z) {
    GLint loc = get_uloc(name);
    if (loc != -1) glUniform3f(loc, x, y, z);
  }
  void Shader::set_vec4(const std::string& name, float x, float y, float z, float w) {
    GLint loc = get_uloc(name);
    if (loc != -1) glUniform4f(loc, x, y, z, w);
  }
}
