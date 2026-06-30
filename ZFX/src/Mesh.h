#pragma once
#include "Vertex.h"
#include <glm/glm.hpp>


namespace ZFX
{
    using Indices = std::vector<uint32_t>;

    class Mesh
    {
        Mesh(const Vertices& vertices, const Indices& indices, unsigned numBuffers);

    public:
        Mesh(const Vertices& vertices, const Indices& indices);
        Mesh(const Vertices& vertices, const Indices& indices, const std::vector<glm::mat4>& modelMatrices);
        Mesh(const Mesh& other) = delete;
        Mesh(Mesh&&) = default;
        Mesh& operator=(const Mesh& other) = delete;
        ~Mesh();

        void draw() const;
        void draw(GLsizei amount) const;

        void updateModels(const std::vector<glm::mat4>& modelMatrices);
        GLuint getInstanceBufferID() const { return m_vertexBuffers[INSTANCE_BUFFER]; }

    private:
        enum
        {
            VERTEX_BUFFER,
            INDEX_BUFFER,
            INSTANCE_BUFFER,

            MAX_NUM_BUFFERS
        };

        GLsizei  m_numIndices;
        unsigned m_numBuffers;

        GLuint m_vertexArrayObject;
        GLuint m_vertexBuffers[MAX_NUM_BUFFERS];
    };
}
