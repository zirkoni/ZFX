#pragma once
#include "Vertex.h"
#include <GL/glew.h>
#include <vector>


namespace ZFX
{
    using Verteces = std::vector<Vertex>;
    using Indeces = std::vector<uint32_t>;

    class Mesh
    {
    public:
        Mesh(const Verteces& vertices, const Indeces& indeces);
        Mesh(const Mesh& other) = delete;
        Mesh& operator=(const Mesh& other) = delete;
        ~Mesh();

        void draw();

    private:
        enum
        {
            VERTEX_BUFFER,
            INDEX_BUFFER,

            NUM_BUFFERS
        };

        GLsizei m_numIndeces;
        GLuint m_vertexArrayObject;
        GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    };
}

