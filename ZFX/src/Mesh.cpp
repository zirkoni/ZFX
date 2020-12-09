#include "Mesh.h"
#include "zfxdefs.h"
#include <algorithm>


ZFX::Mesh::Mesh(const Verteces& vertices, const Indeces& indeces) :
    m_numIndeces{ (GLsizei)indeces.size() }, m_vertexArrayObject{ 0 }
{
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // move data to GPU
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertices.data().size() * sizeof(float), vertices.data().data(), GL_STATIC_DRAW);
    
    // tell GPU how to interpret the data
    GLuint index = 0;
    uint64_t offset = 0;
    uint32_t numElementsPerVertex = vertices.numElementsPerVertex();

    for (const auto& attribute : vertices.attributes())
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, attribute.numElements, GL_FLOAT, GL_FALSE,
            numElementsPerVertex * sizeof(float), (GLvoid*)offset);
        
        ++index;
        offset += attribute.numElements * sizeof(float);
    }

    // indeces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndeces * sizeof(indeces.at(0)), indeces.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ZFX::Mesh::~Mesh()
{
    glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void ZFX::Mesh::draw()
{
    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, m_numIndeces, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


