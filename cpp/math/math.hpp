#pragma once


extern "C" float trunc(float);
extern "C" float sqrt(float);
extern "C" float sin(float);


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