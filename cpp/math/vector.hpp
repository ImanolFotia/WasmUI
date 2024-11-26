#pragma once

#include "math.hpp"

namespace Math {
struct vec3 {
  vec3() = default;
  vec3(float v) : x{v}, y{v}, z{v} {}

  vec3(float x, float y, float z) : x{x}, y{y}, z{z} {}

  union {
    struct {
      float x = 0;
      float y = 0;
      float z = 0;
    };
    float data[3];
  };

  vec3 operator+(vec3 other) { return {x + other.x, y + other.y, z + other.z}; }

  vec3 operator-(vec3 other) { return {x - other.x, y - other.y, z - other.z}; }

  vec3 operator*(vec3 other) { return {x * other.x, y * other.y, z * other.z}; }

  vec3 operator/(vec3 other) { return {x / other.x, y / other.y, z / other.z}; }

  float length() { return sqrt(dot(*this)); }

  [[maybe_unused]] vec3 normalize() {
    float l = length();
    *this = *this / l;
    return *this;
  }

  float dot(vec3 other) { return x * other.x + y * other.y + z * other.z; }

  vec3 cross(vec3 o) {
    return {y*o.z-o.y*z, o.x*z-x*o.z, x*o.y-o.x*y};
  }
};


struct vec4 {
  vec4() = default;
  vec4(float v) : x{v}, y{v}, z{v} {}

  vec4(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} {}

  union {
    struct {
      float x = 0;
      float y = 0;
      float z = 0;
      float w = 0;
    };
    float data[4];
  };

  vec4 operator+(vec4 other) { return {x + other.x, y + other.y, z + other.z, w + other.w}; }

  vec4 operator-(vec4 other) { return {x - other.x, y - other.y, z - other.z, w - other.w}; }

  vec4 operator*(vec4 other) { return {x * other.x, y * other.y, z * other.z, w * other.w}; }

  vec4 operator/(vec4 other) { return {x / other.x, y / other.y, z / other.z, w / other.w}; }

  float length() { return sqrt(dot(*this)); }

  [[maybe_unused]] vec4 normalize() {
    float l = length();
    *this = *this / l;
    return *this;
  }

  float dot(vec4 other) { return x * other.x + y * other.y + z * other.z + w * other.w; }

  vec4 cross(vec4 o) {
    return {y*o.z-o.y*z, o.x*z-x*o.z, x*o.y-o.x*y, 1.0f};
  }
};

/*
static vec3 operator+(vec3 a, vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
static vec3 operator-(vec3 a, vec3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
static vec3 operator*(vec3 a, vec3 b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
static vec3 operator/(vec3 a, vec3 b) { return {a.x / b.x, a.y / b.y, a.z / b.z}; }
*/
} // namespace Math