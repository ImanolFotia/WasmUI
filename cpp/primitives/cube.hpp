#pragma once

#include "primitive.hpp"

namespace engine
{

    class Cube : public Primitive
    {
        float mUVMultiplier = 1.0f;
        Math::vec3 m_pScale = Math::vec3(1.0f);
        Math::vec4 m_pColor = Math::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    public:
        Cube()
        {
            pInit();
        }

        Cube(Math::vec3 scale) : m_pScale(scale){
            pInit();
        }

        Cube(Math::vec4 color) : m_pColor(color)
        {
            pInit();
        }


        void pInit() {
            m_pType = DrawableType::CUBE;
            using vtx = Math::Vertex;
            using v4 = Math::vec4;
            using v3 = Math::vec3;
            using v2 = Math::vec2;

            reset_arena(&primitive_vertex_arena);
            m_pMesh.Vertices = (Math::Vertex *)alloc_arena(&primitive_vertex_arena,
                                                        sizeof(Math::Vertex) * 36);
            reset_arena(&primitive_index_arena);
            m_pMesh.Indices = (uint32_t *)alloc_arena(&primitive_index_arena,
                                                        sizeof(uint32_t) * 36);
            m_pMesh.num_vertices = 0;
            m_pMesh.num_indices = 0;

            // m_pMesh.Vertices = {
            //  back face
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 0.0f), v3(0.0f, 0.0f, -1.0f)); // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, -0.5f) * m_pScale, v2(1.0f, 1.0f), v3(0.0f, 0.0f, -1.0f));   // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, -0.5f) * m_pScale, v2(1.0f, 0.0f), v3(0.0f, 0.0f, -1.0f));  // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, -0.5f) * m_pScale, v2(1.0f, 1.0f), v3(0.0f, 0.0f, -1.0f));   // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 0.0f), v3(0.0f, 0.0f, -1.0f)); // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(0.0f, 0.0f, -1.0f));  // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, 0.5f) * m_pScale, v2(0.0f, 0.0f), v3(0.0f, 0.0f, 1.0f)); // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(0.0f, 0.0f, 1.0f));  // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 1.0f), v3(0.0f, 0.0f, 1.0f));   // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 1.0f), v3(0.0f, 0.0f, 1.0f));   // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, 0.5f) * m_pScale, v2(0.0f, 1.0f), v3(0.0f, 0.0f, 1.0f));  // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, 0.5f) * m_pScale, v2(0.0f, 0.0f), v3(0.0f, 0.0f, 1.0f)); // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(-1.0f, 0.0f, 0.0f));   // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, -0.5f) * m_pScale, v2(1.0f, 1.0f), v3(-1.0f, 0.0f, 0.0f));  // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(-1.0f, 0.0f, 0.0f)); // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(-1.0f, 0.0f, 0.0f)); // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, 0.5f) * m_pScale, v2(0.0f, 0.0f), v3(-1.0f, 0.0f, 0.0f));  // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(-1.0f, 0.0f, 0.0f));   // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(1.0f, 0.0f, 0.0f));   // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(1.0f, 0.0f, 0.0f)); // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, -0.5f) * m_pScale, v2(1.0f, 1.0f), v3(1.0f, 0.0f, 0.0f));  // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(1.0f, 0.0f, 0.0f)); // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(1.0f, 0.0f, 0.0f));   // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, 0.5f) * m_pScale, v2(0.0f, 0.0f), v3(1.0f, 0.0f, 0.0f));  // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(0.0f, -1.0f, 0.0f)); // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, -0.5f) * m_pScale, v2(1.0f, 1.0f), v3(0.0f, -1.0f, 0.0f));  // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(0.0f, -1.0f, 0.0f));   // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, -0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(0.0f, -1.0f, 0.0f));   // bottom-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, 0.5f) * m_pScale, v2(0.0f, 0.0f), v3(0.0f, -1.0f, 0.0f));  // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, -0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(0.0f, -1.0f, 0.0f)); // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(0.0f, 1.0f, 0.0f)); // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(0.0f, 1.0f, 0.0f));   // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, -0.5f) * m_pScale, v2(1.0f, 1.0f), v3(0.0f, 1.0f, 0.0f));  // top-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(0.5f, 0.5f, 0.5f) * m_pScale, v2(1.0f, 0.0f), v3(0.0f, 1.0f, 0.0f));   // bottom-right
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, -0.5f) * m_pScale, v2(0.0f, 1.0f), v3(0.0f, 1.0f, 0.0f)); // top-left
            m_pMesh.Vertices[m_pMesh.num_vertices++] = vtx(2.0f * v3(-0.5f, 0.5f, 0.5f) * m_pScale, v2(0.0f, 0.0f), v3(0.0f, 1.0f, 0.0f));  // bottom-left
            //};
            // Generate tangent space vectors


            for (int i = 0; i < m_pMesh.num_vertices; i += 3)
            {
                m_pMesh.addTriangle(i, i + 1, i + 2);
            }


            //generateTangentSpaceVectors();
        }

    };

} // namespace Epsilon