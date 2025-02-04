#pragma once

#include "math.hpp"
#include <stdint.h>

namespace Math {

template <uint32_t comps, typename T> struct vec {};

template <typename T> struct vec<2, T> {

  vec() = default;

  vec(T v) : x{v}, y{v} {}

  vec(T x, T y) : x{x}, y{y}{}

  union {
    struct {
      T x, y;
    };
    struct {
      T r, g;
    };
    struct {
      T s, t;
    };
    T data[2];
  };

  float length() { return sqrt(dot(*this)); }

  vec operator+(vec other) { return {x + other.x, y + other.y}; }
  vec operator+=(float other) {  *this = {x + other, y + other}; return *this; }

  vec operator-(vec other) { return {x - other.x, y - other.y}; }
  vec operator-=(float other) {  *this = {x - other, y - other}; return *this; }

  vec operator*(vec other) { return {x * other.x, y * other.y}; }
  vec operator*=(float other) {  *this = {x * other, y * other}; return *this; }

  vec operator/(vec other) { return {x / other.x, y / other.y}; }

  bool operator==(vec other) { return x == other.x && y == other.y; }

  vec operator-() { return {-x, -y}; }

  T dot(vec other) { return x * other.x + y * other.y; }

  T ceil() { *this = {ceil(x), ceil(y)}; return *this; }
  T floor() { *this = {floor(x), floor(y)}; return *this; }

  vec cross(vec o) {
    //return {y * o.z - o.y * z, o.x * z - x * o.z, x * o.y - o.x * y};
  }

  [[maybe_unused]] vec normalize() {
    float l = length();
    *this = *this / l;
    return *this;
  }
};

template <typename T> struct vec<3, T> {

  vec() = default;

  vec(T v) : x{v}, y{v}, z{v} {}

  vec(T x, T y, T z) : x{x}, y{y}, z{z} {}

  union {
    struct {
      T x, y, z;
    };
    struct {
      T r, g, b;
    };
    T data[3];
  };

  float length() { return sqrt(dot(*this)); }

  vec operator+(vec other) { return {x + other.x, y + other.y, z + other.z}; }

  vec operator+(float other) { return {x + other, y + other, z + other}; }

  vec operator+=(float other) {  *this = {x + other, y + other, z + other}; return *this; }

  vec operator-(vec other) { return {x - other.x, y - other.y, z - other.z}; }

  vec operator-(float other) { return {x - other, y - other, z - other}; }
  vec operator-=(float other) {  *this = {x - other, y - other, z - other}; return *this; }
  vec operator-=(vec other) {  *this = {x - other.x, y - other.y, z - other.z}; return *this; }

  vec operator*(vec other) { return {x * other.x, y * other.y, z * other.z}; }
  vec operator*=(float other) {  *this = {x * other, y * other, z * other}; return *this; }
  vec operator*=(vec other) {  *this = {x * other.x, y * other.y, z * other.z}; return *this; }

  vec operator*(float other) { return {x * other, y * other, z * other}; }

  vec operator/(vec other) { return {x / other.x, y / other.y, z / other.z}; }

  vec operator/(float other) { return {x / other, y / other, z / other}; }

  bool operator==(vec other) { return x == other.x && y == other.y && z == other.z; }

  vec operator-() { return {-x, -y, -z}; }

  T dot(vec other) { return x * other.x + y * other.y + z * other.z; }

  T ceil() { *this = {ceil(x), ceil(y), ceil(z)}; return *this; }

  T floor() { *this = {floor(x), floor(y), floor(z)}; return *this; }

  vec cross(vec o) {
    return {y * o.z - o.y * z, o.x * z - x * o.z, x * o.y - o.x * y};
  }

  [[maybe_unused]] vec normalize() {
    float l = length();
    *this = *this / l;
    return *this;
  }
};


static vec<3, float> operator*(float other, vec<3, float> v) { return {v.x * other, v.y * other, v.z * other}; }

template <typename T> struct vec<4, T> {

  vec() = default;

  vec(T v) : x{v}, y{v}, z{v}, w{v} {}

  vec(T x, T y, T z, T w) : x{x}, y{y}, z{z}, w{w} {}

  union {
    struct {
      T x, y, z, w;
    };
    struct {
      T r, g, b, a;
    };
    T data[4];
  };

  float length() { return sqrt(dot(*this)); }

  vec operator+(vec other) {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
  }

  vec operator-(vec other) {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
  }
  vec operator-=(float other) {  *this = {x - other, y - other, z - other, w - other}; return *this; }
  vec operator-=(vec other) {  *this = {x - other.x, y - other.y, z - other.z, w - other.w}; return *this; }

  vec operator*(vec other) {
    return {x * other.x, y * other.y, z * other.z, w * other.w};
  }
  vec operator*=(vec other) {  *this = {x * other.x, y * other.y, z * other.z, w * other.w}; return *this; }
  vec operator*=(float other) {  *this = {x * other, y * other, z * other, w * other}; return *this; }

  vec operator/(vec other) {
    return {x / other.x, y / other.y, z / other.z, w / other.w};
  }

  bool operator==(vec other) { return x == other.x && y == other.y && z == other.z && w == other.w; }

  vec operator-() { return {-x, -y, -z, -w}; }

  float dot(vec other) {
    return x * other.x + y * other.y + z * other.z + w * other.w;
  }

  T ceil() { *this = {ceil(x), ceil(y), ceil(z), ceil(w)}; return *this; }

  T floor() { *this = {floor(x), floor(y), floor(z), floor(w)}; return *this; }

  vec cross(vec o) {
    return {y * o.z - o.y * z, o.x * z - x * o.z, x * o.y - o.x * y, 1.0f};
  }

  [[maybe_unused]] vec normalize() {
    float l = length();
    *this = *this / l;
    return *this;
  }
};

using  vec2 = vec<2, float>;
using dvec2 = vec<2, double>;
using ivec2 = vec<2, int>;
using  vec3 = vec<3, float>;
using dvec3 = vec<3, double>;
using ivec3 = vec<3, int>;
using  vec4 = vec<4, float>;
using dvec4 = vec<4, double>;
using ivec4 = vec<4, int>;

/*
static vec3 operator+(vec3 a, vec3 b) { return {a.x + b.x, a.y + b.y, a.z +
b.z}; } static vec3 operator-(vec3 a, vec3 b) { return {a.x - b.x, a.y - b.y,
a.z - b.z}; } static vec3 operator*(vec3 a, vec3 b) { return {a.x * b.x, a.y *
b.y, a.z * b.z}; } static vec3 operator/(vec3 a, vec3 b) { return {a.x / b.x,
a.y / b.y, a.z / b.z}; }
*/
} // namespace Math