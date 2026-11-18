#include <hellbliss/core/window.hpp>

#include <glad/gl.h>

#include <stdexcept>

namespace hellbliss::core {
  void viewport_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
  }
  
  Window::Window(int width, int height, const char* title) {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
      glfwTerminate();
      throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
      glfwDestroyWindow(window);
      glfwTerminate();

      throw std::runtime_error("failed to initialize GLAD");
    }

    glfwSetFramebufferSizeCallback(window, viewport_size_callback);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    glViewport(0, 0, fb_width, fb_height);
  }

  Window::~Window() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
  }

  bool Window::should_close() const {
    return glfwWindowShouldClose(window);
  }

  void Window::poll_events() {
    glfwPollEvents();
  }

  void Window::swap_buffers() {
    glfwSwapBuffers(window);
  }

  vec2i Window::get_mouse_pos() const {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    return {(int)x, (int)y};
  }

  bool Window::is_button_pressed(int button) const {
    return glfwGetMouseButton(window, button);
  }
}
