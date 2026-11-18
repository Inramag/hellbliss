#include <hellbliss/object.hpp>

#include <hellbliss/scene.hpp>

namespace hellbliss {
  vec3i Object::gpos() const {
    return parent_ ? parent_->gpos() + pos : pos;
  }
  void Object::set_gpos(vec3i ngpos) {
    pos = parent_ ? ngpos - parent_->gpos() : ngpos;
  }

  void Object::set_parent(Object* parent) {
    scene_->set_parent(*this, parent);
  }
  void Object::destroy() {
    scene_->request_destroy(this);
  }

  void Object::start() {
    for (auto& script : scripts_) script->start();
    for (auto& obj : children_) obj->start();
  }

  void Object::update(float dt) {
    for (auto& script : scripts_) script->update(dt);
    for (auto& obj : children_) obj->update(dt);
  }

  void Object::render() {
    for (auto& script : scripts_) script->render();
  }

  bool Object::is_descendant_of(Object* obj) {
    for (auto& child : children_)
      if (child.get() == obj || child->is_descendant_of(obj)) return true;
    return false;
  }
}
