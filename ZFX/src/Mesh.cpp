#include "Mesh.h"
#include "zfxdefs.h"
#include <algorithm>
#include <cstring>
#include <SDL2/SDL.h>


ZFX::Mesh::Mesh(const Verteces& vertices, const Indeces& indeces, unsigned numBuffers) :
    m_numIndeces{ (GLsizei)indeces.size() }, m_numBuffers{numBuffers}, m_vertexArrayObject{ 0 }
{
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // move data to GPU
    glGenBuffers(numBuffers, m_vertexBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertices.data().size() * sizeof(float), vertices.data().data(), GL_STATIC_DRAW);
    
    // tell GPU how to interpret the data
    GLuint index = 0;
    uint64_t offset = 0;
    uint32_t numElementsPerVertex = vertices.numElementsPerVertex();

    for (const auto& size : vertices.attributes())
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
            numElementsPerVertex * sizeof(float), (GLvoid*)offset);
        
        ++index;
        offset += size * sizeof(float);
    }

    // indeces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffers[INDEX_BUFFER]);
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

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[INSTANCE_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(modelMatrices.at(0)), modelMatrices.data(), GL_DYNAMIC_DRAW);

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
    glDeleteBuffers(m_numBuffers, m_vertexBuffers);
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

void ZFX::Mesh::updateModels(const std::vector<glm::mat4>& modelMatrices)
{
    if(m_numBuffers > INSTANCE_BUFFER)
    {
#if 0
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[INSTANCE_BUFFER]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, modelMatrices.size() * sizeof(modelMatrices.at(0)), modelMatrices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[INSTANCE_BUFFER]);
        void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        std::memcpy(ptr, modelMatrices.data(), modelMatrices.size() * sizeof(modelMatrices.at(0)));
        glUnmapBuffer(GL_ARRAY_BUFFER);
#endif
    } else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ZFX::Mesh::updateModels called while numBuffers=%u. Do nothing!", m_numBuffers);
    }
}
