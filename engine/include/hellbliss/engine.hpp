#pragma once

#include <hellbliss/core/window.hpp>
#include <hellbliss/input/mouse.hpp>
#include <hellbliss/render/renderer.hpp>
#include <hellbliss/resources.hpp>
#include <hellbliss/scene.hpp>


namespace hellbliss {
  class Engine {
  public:
    Engine();
    ~Engine() = default;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void create_window(vec2i size, const char* title);
    void run();

    template<typename T>
    Object& create_object() {
      static_assert(std::is_base_of_v<Script, T>, "T must inherit fron hellbliss::Script");
      return scene_.create_object<T>();
    }
    
    core::Window& window() { return *window_; }
    input::Mouse& mouse() { return *mouse_; }
    render::Renderer& renderer() { return renderer_; }
    Resources& resources() { return *resources_; }
    Scene& scene() { return scene_; }

  private:
    std::unique_ptr<core::Window> window_;
    std::unique_ptr<input::Mouse> mouse_;
    render::Renderer renderer_;
    std::unique_ptr<Resources> resources_;
    Scene scene_;
  };
}
