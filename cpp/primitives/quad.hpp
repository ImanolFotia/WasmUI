//
// Created by solaire on 12/14/22.
//

#ifndef LEARNINGVULKAN_QUAD_HPP
#define LEARNINGVULKAN_QUAD_HPP
#include "primitive.hpp"
namespace engine {
class Quad : public Primitive {


public:

  Quad() {
    reset_arena(&primitive_vertex_arena);
    m_pMesh.Vertices = (Math::Vertex *)alloc_arena(&primitive_vertex_arena,
                                                   sizeof(Math::Vertex) * 6);
    reset_arena(&primitive_index_arena);
    m_pMesh.Indices = (uint32_t *)alloc_arena(&primitive_index_arena,
                                              sizeof(uint32_t) * 6);
    m_pMesh.num_vertices = 0;
    m_pMesh.num_indices = 0;

  Math::vec2 m_pUvMultiplier = Math::vec2(1.0f);
  Math::vec3 size = Math::vec3(1.0f);

    m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(v3(-1.0f,0.0f, -1.0f) * 0.5f * size, v2(1.0f, 1.0f) * m_pUvMultiplier, v3(0.0f, 1.0f, 0.0f)); // bottom-left
    m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(v3(1.0f,0.0f, -1.0f) * 0.5f * size, v2(0.0f, 1.0f) * m_pUvMultiplier, v3(0.0f, 1.0f, 0.0f)); // bottom-right
    m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(v3(1.0f,0.0f, 1.0f) * 0.5f * size, v2(0.0f, 0.0f) * m_pUvMultiplier, v3(0.0f, 1.0f, 0.0f)); // top-right
    m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(v3(-1.0f,0.0f, 1.0f) * 0.5f * size, v2(1.0f, 0.0f) * m_pUvMultiplier, v3(0.0f, 1.0f, 0.0f)); // top-left

    m_pMesh.addTriangle(2, 1, 0);
    m_pMesh.addTriangle(0, 3, 2);

    std::printf("% vertices", m_pMesh.num_vertices);

    std::printf("% indices", m_pMesh.num_indices);
  }
/*
  Quad(uint16_t tesselation, Math::vec2 uvMultiplier = Math::vec2(1.0f), Math::vec3 size = Math::vec3(1.0f))
      :  m_pUvMultiplier(uvMultiplier), m_pTesselation(tesselation) {


    reset_arena(&primitive_vertex_arena);
    m_pMesh.Vertices = (Math::Vertex *)alloc_arena(&primitive_vertex_arena,
                                                   sizeof(Math::Vertex) * 6);
    reset_arena(&primitive_index_arena);
    m_pMesh.Indices = (uint32_t *)alloc_arena(&primitive_index_arena,
                                              sizeof(uint32_t) * 6);
    m_pMesh.num_vertices = 0;
    m_pMesh.num_indices = 0;

      pInit(2);*/
//
//    if (tesselation <= 2) {
//      pInit(size);
//      return;
//    }
//
//
//    float step = 1.0f / ((float)tesselation + 1);
//    int numVertices = (tesselation) * (tesselation);
//    bounds.min = Math::vec3(100000.0f);
//    bounds.max = Math::vec3(-100000.0f);
//
//
//    for (int i = 0; i <= tesselation; i++) {
//      for (int j = 0; j <= tesselation; j++) {
//        float x = (float)j;
//        float y = (float)i;
//        v3 position = v3(x * step, 0.0f, y * step);
//        v2 uv = v2(x, y) * uvMultiplier;
//        v3 normal = v3(0.0f, 1.0f, 0.0f);
//
//        position -= v3(0.5f, 0.0f, 0.5f);
//        position *= size;
//
//        if (position.x < bounds.min.x)
//          bounds.min.x = position.x;
//        if (position.x > bounds.max.x)
//          bounds.max.x = position.x;
//
//        if (position.z < bounds.min.z)
//          bounds.min.z = position.z;
//        if (position.z > bounds.max.z)
//          bounds.max.z = position.z;
//
//        m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(position, uv, normal);
//      }
//    }
//    bounds.min.y = 0.0f;
//    bounds.max.y = 1.0f;
//
//    bounds.center = bounds.max - bounds.min;
//
//    int edge = 0;
//    for (int i = 0; i < numVertices + tesselation; i++) {
//      if (edge >= tesselation) {
//        edge = 0;
//        continue;
//      }
//      m_pMesh.addTriangle(i, i + 1, i + tesselation + 1);
//      m_pMesh.addTriangle(i + tesselation + 1, i + 1, i + tesselation + 2);
//      edge++;
//    }

/*  }*/


private:

};
} // namespace engine

#endif // LEARNINGVULKAN_QUAD_HPP
