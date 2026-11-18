#pragma once

#include <hellbliss/engine.hpp>

namespace scripts {
  class Init : public hellbliss::Script {
  private:
    void start() override {
      Script::start();
      auto& engine = object().engine();
      auto* shader = engine.resources().load_shader("default", "default");
      if (shader) engine.renderer().initialize(*shader);
    }
  };
}
