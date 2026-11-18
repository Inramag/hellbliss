#pragma once

namespace hellbliss {
  class Object;
  
  class Script {
    friend class Object;

  public:
    virtual ~Script() = default;
  
    Object& object() { return *object_; }

  protected:
    virtual void start() {}
    virtual void update(float dt) {}

    virtual void render() {}

    Object* object_ = nullptr;
  };
}
