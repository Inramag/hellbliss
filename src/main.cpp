#include <hellbliss/engine.hpp>

#include <scripts/init.hpp>

int main() {
  hellbliss::Engine engine;

  engine.create_window({1280, 720}, "test-game");
  engine.create_object<scripts::Init>();

  engine.run();

  return 0;
}
