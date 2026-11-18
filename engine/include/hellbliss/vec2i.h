#pragma once

struct vec2i {
  int x = 0, y = 0;

  vec2i operator+(const vec2i& other) const {
    return {x + other.x, y + other.y};
  }

  vec2i operator-(const vec2i& other) const {
    return {x - other.x, y - other.y};
  }

  vec2i& operator+=(const vec2i& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  vec2i& operator-=(const vec2i& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
};
