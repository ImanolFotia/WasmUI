#pragma once

#include <string.hpp>

#include "vector.hpp"
#include <stdint.h>

namespace Math {
struct mat4 {
  mat4() = default;
  mat4(float x) {
    std::memset(data, 0, 16 * sizeof(float));
    data[0] = x;
    data[5] = x;
    data[10] = x;
    data[15] = x;
  }
  mat4(vec4 a, vec4 b, vec4 c, vec4 d) {
    uint8_t i = 0;
    data[i++] = a.x;
    data[i++] = a.y;
    data[i++] = a.z;
    data[i++] = a.w;
    data[i++] = b.x;
    data[i++] = b.y;
    data[i++] = b.z;
    data[i++] = b.w;
    data[i++] = c.x;
    data[i++] = c.y;
    data[i++] = c.z;
    data[i++] = c.w;
    data[i++] = d.x;
    data[i++] = d.y;
    data[i++] = d.z;
    data[i++] = d.w;
  }

  mat4(const mat4& other) {
    *this = other;
  }

  mat4 identity() {
    std::memset(data, 0, 16 * sizeof(float));
    data[0] = 1;
    data[5] = 1;
    data[10] = 1;
    data[15] = 1;
    return *this;
  }

  mat4 operator*(const mat4 &o) {
    mat4 out;
    const float *X = data;
    const float *x = o.data;
    vec4 a = vec4(x[0] * X[0] + x[1] * X[4] + x[2] * X[8] + x[3] * X[12],
                  x[0] * X[1] + x[1] * X[5] + x[2] * X[9] + x[3] * X[13],
                  x[0] * X[2] + x[1] * X[6] + x[2] * X[10] + x[3] * X[14],
                  x[0] * X[3] + x[1] * X[7] + x[2] * X[11] + x[3] * X[15]);
    vec4 b = vec4(x[4] * X[0] + x[5] * X[4] + x[6] * X[8] + x[7] * X[12],
                  x[4] * X[1] + x[5] * X[5] + x[6] * X[9] + x[7] * X[13],
                  x[4] * X[2] + x[5] * X[6] + x[6] * X[10] + x[7] * X[14],
                  x[4] * X[3] + x[5] * X[7] + x[6] * X[11] + x[7] * X[15]);
    vec4 c = vec4(x[8] * X[0] + x[9] * X[4] + x[10] * X[8] + x[11] * X[12],
                  x[8] * X[1] + x[9] * X[5] + x[10] * X[9] + x[11] * X[13],
                  x[8] * X[2] + x[9] * X[6] + x[10] * X[10] + x[11] * X[14],
                  x[8] * X[3] + x[9] * X[7] + x[10] * X[11] + x[11] * X[15]);
    vec4 d = vec4(x[12] * X[0] + x[13] * X[4] + x[14] * X[8] + x[15] * X[12],
                  x[12] * X[1] + x[13] * X[5] + x[14] * X[9] + x[15] * X[13],
                  x[12] * X[2] + x[13] * X[6] + x[14] * X[10] + x[15] * X[14],
                  x[12] * X[3] + x[13] * X[7] + x[14] * X[11] + x[15] * X[15]);
    out = mat4(a, b, c, d);
    return out;
  }

  float data[16];
};

static mat4 lookAt(vec3 pos, vec3 target, vec3 up) {
  vec3 f = (pos - target).normalize();
  vec3 r = up.cross(f).normalize();
  vec3 u = r.cross(f).normalize() * vec3(-1.0f);

  float tx = pos.dot(r);
  float ty = pos.dot(u);
  float tz = pos.dot(f);

  return {vec4(r.x, u.x, f.x, 0.0f), vec4(r.y, u.y, f.y, 0.0f),
          vec4(r.z, u.z, f.z, 0.0f), vec4(-tx, -ty, -tz, 1.0f)};
}

static mat4 perspective(float fov, float aspect, float near, float far) {
  mat4 out;
  out.identity();
  out.data[5] = 1.0f / tan(fov / 2.0f);
  out.data[0] = out.data[5] / aspect;
  out.data[10] = -(far + near) / (far - near);
  out.data[11] = -1;
  out.data[14] = -(2.0 * far * near) / (far - near);
  out.data[15] = 0;

  return out;
}

static mat4 translate(vec3 position) {
  mat4 out;
  out.identity();
  out.data[12] = position.x;
  out.data[13] = position.y;
  out.data[14] = position.z;
  return out;
}


static mat4 scale(vec3 size) {
  mat4 out;
  out.identity();
  out.data[0] = size.x;
  out.data[5] = size.y;
  out.data[10] = size.z;
  return out;
}

static mat4 rotate(float angle, vec3 r) {

  mat4 out;
  out.identity();
  float cosTheta = cos(angle);
  float sinTheta = sin(angle);
  out.data[0] = cosTheta+ r.x*r.x * (1.0 - cosTheta);
  out.data[1] = r.x*r.y * (1.0 - cosTheta) - r.z * sinTheta;
  out.data[2] = r.x*r.z * (1.0 - cosTheta) + r.y * sinTheta;

  out.data[4] = r.y*r.x * (1.0 - cosTheta) + r.z * sinTheta;
  out.data[5] = cosTheta + r.y*r.y * (1.0-cosTheta);
  out.data[6] = r.y*r.z * (1.0 - cosTheta) - r.x * sinTheta;

  out.data[8] = r.z * r.x * (1.0 - cosTheta) - r.y * sinTheta;
  out.data[9] = r.z * r.y * (1.0 - cosTheta) + r.x * sinTheta;
  out.data[10] = cosTheta + r.z*r.z * (1.0 - cosTheta);

  return out;

}
/*
static 
  mat4 operator*(mat4 &am, mat4& bm) {
    mat4 out;
    float *X = am.data;
    float *x = bm.data;
    vec4 a = vec4(x[0] * X[0] + x[1] * X[4] + x[2] * X[8] + x[3] * X[12],
                  x[0] * X[1] + x[1] * X[5] + x[2] * X[9] + x[3] * X[13],
                  x[0] * X[2] + x[1] * X[6] + x[2] * X[10] + x[3] * X[14],
                  x[0] * X[3] + x[1] * X[7] + x[2] * X[11] + x[3] * X[15]);
    vec4 b = vec4(x[4] * X[0] + x[5] * X[4] + x[6] * X[8] + x[7] * X[12],
                  x[4] * X[1] + x[5] * X[5] + x[6] * X[9] + x[7] * X[13],
                  x[4] * X[2] + x[5] * X[6] + x[6] * X[10] + x[7] * X[14],
                  x[4] * X[3] + x[5] * X[7] + x[6] * X[11] + x[7] * X[15]);
    vec4 c = vec4(x[8] * X[0] + x[9] * X[4] + x[10] * X[8] + x[11] * X[12],
                  x[8] * X[1] + x[9] * X[5] + x[10] * X[9] + x[11] * X[13],
                  x[8] * X[2] + x[9] * X[6] + x[10] * X[10] + x[11] * X[14],
                  x[8] * X[3] + x[9] * X[7] + x[10] * X[11] + x[11] * X[15]);
    vec4 d = vec4(x[12] * X[0] + x[13] * X[4] + x[14] * X[8] + x[15] * X[12],
                  x[12] * X[1] + x[13] * X[5] + x[14] * X[9] + x[15] * X[13],
                  x[12] * X[2] + x[13] * X[6] + x[14] * X[10] + x[15] * X[14],
                  x[12] * X[3] + x[13] * X[7] + x[14] * X[11] + x[15] * X[15]);
    out = mat4(a, b, c, d);
    return out;
  }*/


}; // namespace Math