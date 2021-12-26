#pragma once
#include "Vertex.h"
#include <glm/glm.hpp>


namespace ZFX
{
    using Indeces = std::vector<uint32_t>;

    class Mesh
    {
        Mesh(const Verteces& vertices, const Indeces& indeces, unsigned numBuffers);

    public:
        Mesh(const Verteces& vertices, const Indeces& indeces);
        Mesh(const Verteces& vertices, const Indeces& indeces, const std::vector<glm::mat4>& modelMatrices);
        Mesh(const Mesh& other) = delete;
        Mesh& operator=(const Mesh& other) = delete;
        ~Mesh();

        void draw();
        void draw(GLsizei amount);

        void updateModels(const std::vector<glm::mat4>& modelMatrices);

    private:
        enum
        {
            VERTEX_BUFFER,
            INDEX_BUFFER,
            INSTANCE_BUFFER
        };

        GLsizei m_numIndeces;
        GLuint m_vertexArrayObject;
        std::vector<GLuint> m_vertexArrayBuffers;
    };
}

