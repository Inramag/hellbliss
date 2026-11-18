#pragma once

#include <hellbliss/core/window.hpp>

namespace hellbliss::input {
  class Mouse {
  public:
    explicit Mouse(core::Window& window) : window_(window) {}

    vec2i get_pos() {
      return window_.get_mouse_pos();
    }
    bool is_pressed(int button) {
      return window_.is_button_pressed(button);
    }

  private:
    core::Window& window_;
  };
}
