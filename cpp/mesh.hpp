#pragma once

#include "math/vector.hpp"
#include <stdint.h>

namespace Math {

struct Vertex {
    vec3 position;
    vec2 texCoord;
    vec3 normal;
};

struct Mesh {
    uint32_t num_vertices;
    uint32_t num_indices;
    Vertex* Vertices;
    uint32_t* Indices;
    void addTriangle(uint32_t a, uint32_t b, uint32_t c) {
        Indices[num_indices++] = a;
        Indices[num_indices++] = b;
        Indices[num_indices++] = c;
  }
};
}