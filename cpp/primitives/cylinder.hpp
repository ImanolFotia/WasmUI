#pragma once

#include "../arena.hpp"
#include "primitive.hpp"

namespace engine {
class Cylinder : public Primitive {
public:
  Cylinder(uint32_t subdivisions) : mNumSubdivisions(subdivisions) {
    m_pType = DrawableType::SPHERE;
    createGeometry(mNumSubdivisions);
  }

  Cylinder() = default;

private:
  void createGeometry(uint32_t subdivisions) {

    float step = 1.0f / ((float)subdivisions - 1.0);
    int numVertices = subdivisions * subdivisions;

    reset_arena(&primitive_vertex_arena);
    m_pMesh.Vertices = (Math::Vertex *)alloc_arena(&primitive_vertex_arena,
                                                   sizeof(Math::Vertex) * 512);
    reset_arena(&primitive_index_arena);
    m_pMesh.Indices = (uint32_t *)alloc_arena(&primitive_index_arena,
                                              sizeof(uint32_t) * 2048);
    m_pMesh.num_vertices = 0;
    m_pMesh.num_indices = 0;

    // sides
    for (int i = 0; i < subdivisions; i++) {
      for (int j = 0; j < subdivisions; j++) {
        float x = (float)i;
        float y = (float)j;
        v3 position;

        float polar = Math::pi * x * step;
        float azimuth = y * step * Math::two_pi;
        float z = Math::cos(polar);

        position =
            v3(Math::sin(azimuth), (x * step * 2.0) - 1.0, Math::cos(azimuth)) *
            0.5f;

        v2 uv = v2(y * step * 2.0f, x * step) * 2.0f;

        v3 normal = position;
        normal = normal.normalize();
        m_pMesh.Vertices[m_pMesh.num_vertices] = vtx(position, uv, normal);
        m_pMesh.num_vertices++;
      }
    }

    int edge = 0;
    for (int i = 0; i < numVertices - subdivisions; i++) {
      if (edge >= subdivisions - 1) {
        edge = 0;
        continue;
      }
      m_pMesh.addTriangle(i, i + 1, i + subdivisions);
      m_pMesh.addTriangle(i + subdivisions, i + 1, i + subdivisions + 1);
      edge++;
    }

    for (unsigned int i = 0; i < m_pMesh.num_indices; i += 3) {
      Math::vec3 v0 = m_pMesh.Vertices[m_pMesh.Indices[i]].position;
      Math::vec3 v1 = m_pMesh.Vertices[m_pMesh.Indices[i + 1]].position;
      Math::vec3 v2 = m_pMesh.Vertices[m_pMesh.Indices[i + 2]].position;

      if (v0 == v1)
        v0 = -v0;

      Math::vec3 normal = Math::cross(v2 - v0, v1 - v0);

      m_pMesh.Vertices[m_pMesh.Indices[i]].normal = (-normal).normalize();
      m_pMesh.Vertices[m_pMesh.Indices[i + 1]].normal = (-normal).normalize();
      m_pMesh.Vertices[m_pMesh.Indices[i + 2]].normal = (-normal).normalize();
    }

    int cylinder_vertex_offset = m_pMesh.num_vertices;
    int cylinder_index_offset = m_pMesh.num_indices;
    // top & bottom
    for (int i = 0; i <= subdivisions; i++) {
      float x = float(i);
      float azimuth = x * step * Math::two_pi;
      v3 position = v3(Math::sin(azimuth), 1.0, Math::cos(azimuth)) * 0.5f;
      v3 normal = v3(0.0, 1.0, 0.0);
      v2 uv = v2(position.x, position.z) + 0.5f;
      // TODO

      m_pMesh.Vertices[m_pMesh.num_vertices] = vtx(position, uv, normal);
      m_pMesh.num_vertices++;
    }

    int pivot = m_pMesh.num_vertices - 1;

    for (int i = cylinder_vertex_offset; i < pivot - 1; i++) {

      m_pMesh.addTriangle(i, i + 1, pivot);
    }

    cylinder_vertex_offset = m_pMesh.num_vertices;
    cylinder_index_offset = m_pMesh.num_indices;

    for (int i = 0; i <= subdivisions; i++) {
      float x = float(i);
      float azimuth = x * step * Math::two_pi;
      v3 position = v3(Math::sin(azimuth), -1.0, Math::cos(azimuth)) * 0.5f;
      v3 normal = v3(0.0, -1.0, 0.0);
      v2 uv = v2(position.x, position.z) + 0.5f;
      // TODO
      // m_pMesh.Vertices.emplace_back(vtx(position, uv, normal, v4(0.0f),
      // v3(0.0f), v3(0.0f)));

      m_pMesh.Vertices[m_pMesh.num_vertices] = vtx(position, uv, normal);
      m_pMesh.num_vertices++;
    }

    pivot = m_pMesh.num_vertices - 1;

    for (int i = cylinder_vertex_offset; i < pivot - 1; i++) {

      m_pMesh.addTriangle(i, pivot, i + 1);
    }

    std::printf("num vertices: %", m_pMesh.num_vertices);
    std::printf("num indices: %", m_pMesh.num_indices);
  }

  uint32_t mNumSubdivisions = 4;
};
} // namespace engine