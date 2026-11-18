#include <hellbliss/engine.hpp>

#include <unistd.h>

namespace hellbliss {
  Engine::Engine() {
    scene_.engine_ = this;
  }
  
  void Engine::create_window(vec2i size, const char *title) {
    window_ = std::make_unique<core::Window>(size.x, size.y, title);
    mouse_ = std::make_unique<input::Mouse>(*window_);

    std::filesystem::path exe;

    {
      char buffer[4096];
      ssize_t l = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
      if (l < 0) throw std::runtime_error("Failed to get executable path.");
      buffer[l] = '\0';
      exe = buffer;
    }

    resources_ = std::make_unique<Resources>(exe.parent_path() / "resources");

    glfwSwapInterval(0);
  }

  void Engine::run() {
    using clock = std::chrono::steady_clock;

    auto prev = clock::now();

    while (!window_->should_close()) {
      auto now = clock::now();
        float dt = std::chrono::duration<float>(now - prev).count();
        prev = now;

        window_->poll_events();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene_.update(dt);
        scene_.render();

        renderer_.flush();

        window_->swap_buffers();
    }
  }
}
