#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <hellbliss/vec2i.h>

namespace hellbliss::core {
  class Window {
  public:
    Window(int width, int height, const char* title);
    ~Window();

    bool should_close() const;

    void poll_events();
    void swap_buffers();

    vec2i get_mouse_pos() const;
    bool is_button_pressed(int button) const;

  private:
    GLFWwindow* window = nullptr;
  };
}
