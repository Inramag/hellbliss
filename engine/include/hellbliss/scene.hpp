#pragma once

#include <hellbliss/object.hpp>

#include <memory>
#include <vector>

namespace hellbliss {
  class Engine;
  
  class Scene {
  friend class Engine;

  public:
    template<typename... Scripts>
    Object& create_object(std::string name = "object", Object* parent = nullptr, vec3i pos = {0, 0, 0}, vec2i size = {100, 100}) {
      auto object = std::make_unique<Object>(std::move(name), parent, pos, size);

      Object& ref = *object;
      
      ref.engine_ = engine_;
      ref.scene_ = this;

      if (parent) parent->children_.push_back(std::move(object));
      else objects_.push_back(std::move(object));

      (ref.add_script<Scripts>(), ...);

      ref.start();

      return ref;
    }

    void request_destroy(Object* obj);
    void set_parent(Object& obj, Object* new_parent);

  private:
    Engine* engine_ = nullptr;

    void update(float dt);

    void render();
    void collect_render_objects(const std::vector<std::unique_ptr<Object>>& objects, std::vector<Object*>& draw_list);

    std::vector<std::unique_ptr<Object>> objects_;
    std::vector<Object*> pending_destroy_;
  };
}
