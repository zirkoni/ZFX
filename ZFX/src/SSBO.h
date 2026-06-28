#pragma once
#include <GL/glew.h>
#include <vector>

namespace ZFX
{
    class SSBO
    {
    public:

        template<class STORAGE_TYPE>
        SSBO(const std::vector<STORAGE_TYPE>& data, GLuint index)
        {
            glGenBuffers(1, &m_ssbo);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
            glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(STORAGE_TYPE), data.data(), GL_STATIC_DRAW);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_ssbo);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }

        ~SSBO()
        {
            glDeleteBuffers(1, &m_ssbo);
        }

    private:
        GLuint m_ssbo;
    };
}
