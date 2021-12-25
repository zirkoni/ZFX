#include "Mesh.h"
#include "zfxdefs.h"
#include <algorithm>


ZFX::Mesh::Mesh(const Verteces& vertices, const Indeces& indeces, unsigned numBuffers) :
    m_numIndeces{ (GLsizei)indeces.size() }, m_vertexArrayObject{ 0 }, m_vertexArrayBuffers(numBuffers, 0)
{
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // move data to GPU
    glGenBuffers(numBuffers, m_vertexArrayBuffers.data());
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.at(VERTEX_BUFFER));
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers.at(INDEX_BUFFER));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndeces * sizeof(indeces.at(0)), indeces.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ZFX::Mesh::Mesh(const Verteces &vertices, const Indeces &indeces): Mesh{ vertices, indeces, 2 }
{
}

ZFX::Mesh::Mesh(const Verteces &vertices, const Indeces &indeces, const std::vector<glm::mat4>& modelMatrices) :
    Mesh{ vertices, indeces, 3 }
{
    // set instance vertex attribute (with divisor 1)
    // set attribute pointers for matrix (4 x vec4)

    glBindVertexArray(m_vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.at(INSTANCE_BUFFER));
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(modelMatrices.at(0)), modelMatrices.data(), GL_STATIC_DRAW);

    uint64_t offset = 0;

    for (unsigned index = vertices.attributes().size(); index < vertices.attributes().size() + 4; ++index)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)offset);
        glVertexAttribDivisor(index, 1);

        offset += sizeof(glm::vec4);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ZFX::Mesh::~Mesh()
{
    glDeleteBuffers(m_vertexArrayBuffers.size(), m_vertexArrayBuffers.data());
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void ZFX::Mesh::draw()
{
    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, m_numIndeces, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ZFX::Mesh::draw(GLsizei amount)
{
    glBindVertexArray(m_vertexArrayObject);
    glDrawElementsInstanced(GL_TRIANGLES, m_numIndeces, GL_UNSIGNED_INT, 0, amount);
    glBindVertexArray(0);
}
