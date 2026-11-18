#pragma once

#include <hellbliss/vec2i.h>

struct vec3i {
  int x = 0, y = 0, z = 0;

  vec3i() = default;
  vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
  vec3i(const vec2i& xy, int z = 0) : x(xy.x), y(xy.y), z(z) {}

  vec2i xy() const {
    return {x, y};
  }

  vec3i operator+(const vec3i& other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  vec3i operator-(const vec3i& other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  vec3i& operator+=(const vec3i& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  vec3i& operator-=(const vec3i& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
};
