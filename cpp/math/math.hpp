#pragma once

namespace Math {

extern "C" float trunc(float);
extern "C" float sqrt(float);
extern "C" float sin(float);
extern "C" float cos(float);
extern "C" float tan(float);
extern "C" float log10(float);
extern "C" float pow(float, float);
extern "C" float ceil(float);
extern "C" float floor(float);
extern "C" float rand();

constexpr const double pi = 3.14159265358979323846264;
constexpr const double two_pi = pi * 2.0;

static float radians(float angle) {
  return angle * (pi/180.0f);
}

template <typename T>
T abs(T x) {
  return x > 0 ? x : -x;
}

static float fmodf(float x, float y) { return x - trunc(x / y) * y; }

static float min(float a, float b) {
  if (a > b)
    return a;
  return b;
}

static float max(float a, float b) {
  if (a > b)
    return b;
  return a;
}

template <typename T>
T cross(T a, T b) {
  return a.cross(b);
}

template <typename T>
float distance(T a, T b) {
  return (a - b).length();
}

template <typename T>
float ceil(T a) {
  return a.ceil();
}

template <typename T>
float floor(T a) {
  return a.floor();
}

template <typename T>
T lerp(T v0, T v1, T t) {
  return (1 - t) * v0 + t * v1;
}

template <typename T>
T mix(T v0, T v1, T t) {
  return (1 - t) * v0 + t * v1;
}
}