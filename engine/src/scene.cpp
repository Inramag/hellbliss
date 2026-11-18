#include <hellbliss/scene.hpp>

#include <algorithm>

namespace hellbliss {
  void Scene::request_destroy(Object* obj) {
    if (!obj) return;
    if (std::find(pending_destroy_.begin(), pending_destroy_.end(), obj) == pending_destroy_.end()) pending_destroy_.push_back(obj);
  }

  void Scene::set_parent(Object& obj, Object* new_parent) {
    if (new_parent == &obj || new_parent == obj.parent_) return;
    if (new_parent && obj.is_descendant_of(new_parent)) return;

    auto* old_parent = obj.parent_;
    std::unique_ptr<Object> moved;

    if (old_parent) {
      auto it = std::find_if(old_parent->children_.begin(), old_parent->children_.end(), [&obj](const std::unique_ptr<Object>& object) { return object.get() == &obj; });
      if (it == old_parent->children_.end()) return;
      else {
        moved = std::move(*it);
        old_parent->children_.erase(it);
      }
    } else {
      auto it = std::find_if(objects_.begin(), objects_.end(), [&obj](const std::unique_ptr<Object>& object_ptr) { return object_ptr.get() == &obj; });
      if (it == objects_.end()) return;
      else {
        moved = std::move(*it);
        objects_.erase(it);
      }
    }

    if (new_parent) {
      obj.parent_ = new_parent;
      new_parent->children_.push_back(std::move(moved));
    } else {
      obj.parent_ = nullptr;
      objects_.push_back(std::move(moved));
    }
  }

  void Scene::collect_render_objects(const std::vector<std::unique_ptr<Object>>& objects, std::vector<Object*>& draw_list) {
    for (auto& obj : objects) {
      draw_list.push_back(obj.get());
      collect_render_objects(obj->children_, draw_list);
    }
  }

  void Scene::update(float dt) {
    for (auto& obj : objects_) obj->update(dt);
    for (auto& obj : pending_destroy_) {
      auto it = std::find_if(objects_.begin(), objects_.end(), [obj](const std::unique_ptr<Object>& ptr) { return ptr.get() == obj; });
      if (it != objects_.end()) objects_.erase(it);
    }
  }

  void Scene::render() {
    std::vector<Object*> draw_list;
    collect_render_objects(objects_, draw_list);
    std::stable_sort(draw_list.begin(), draw_list.end(), [](const Object* a, const Object* b) { return a->gpos().z < b->gpos().z; });
    for (auto* obj : draw_list) obj->render();
  }  
}
