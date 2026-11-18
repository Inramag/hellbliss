#pragma once

#include <hellbliss/script.hpp>
#include <hellbliss/vec2i.h>
#include <hellbliss/vec3i.h>

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace hellbliss {
  class Engine;
  class Scene;
  
  class Object final {
    friend Scene;
    
  public:
    Object(std::string name = "object", Object* parent = nullptr, vec3i pos = {}, vec2i size = {100, 100}) : name(std::move(name)), parent_(parent), pos(pos), size(size) {}

    virtual ~Object() = default;

    std::string name;
    vec3i pos;
    vec2i size;

    vec3i gpos() const;
    void set_gpos(vec3i ngpos);

    void set_parent(Object* parent);
    void destroy();

    template<typename T>
    T& add_script() {
      static_assert(std::is_base_of_v<Script, T>);

      if (auto* existing = get_script<T>()) return *existing;

      auto script = std::make_unique<T>();
      script->object_ = this;

      T& ref = *script;
      scripts_.push_back(std::move(script));
      return ref;
    }

    template<typename T>
    T* get_script() {
      static_assert(std::is_base_of_v<Script, T>);

      for (auto& script : scripts_)
        if (auto* result = dynamic_cast<T*>(script.get()))
          return result;

      return nullptr;
    }

    Engine& engine() { return *engine_; }
    Scene& scene() { return *scene_; }

private:
    friend class Engine;

    void start();
    void update(float dt);
    void render();

    bool is_descendant_of(Object* obj);

    Object* parent_ = nullptr;
    std::vector<std::unique_ptr<Object>> children_;

    std::vector<std::unique_ptr<Script>> scripts_;
    Engine* engine_ = nullptr;
    Scene* scene_ = nullptr;
  };
}
