#pragma once


extern "C" float trunc(float);
extern "C" float sqrt(float);
extern "C" float sin(float);
extern "C" float cos(float);
extern "C" float tan(float);

constexpr const double pi = 3.141592741012573;

static float radians(float angle) {
  return angle * (pi/180.0f);
}

float fmodf(float x, float y) { return x - trunc(x / y) * y; }

float min(float a, float b) {
  if (a > b)
    return a;
  return b;
}

float max(float a, float b) {
  if (a > b)
    return b;
  return a;
}