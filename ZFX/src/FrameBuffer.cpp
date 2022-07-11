#include "FrameBuffer.h"
#include <stdexcept>
#include <string>


ZFX::FrameBuffer::FrameBuffer(): m_fbo{0}, m_textureBuffer{0}, m_rbo{0}
{
    glGenFramebuffers(1, &m_fbo);
}

void ZFX::FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void ZFX::FrameBuffer::bindDefault()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void ZFX::FrameBuffer::attachTextureBuffer(GLsizei width, GLsizei height)
{
    bind();

    glGenTextures(1, &m_textureBuffer);
    glBindTexture(GL_TEXTURE_2D, m_textureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureBuffer, 0);

    bindDefault();
}

void ZFX::FrameBuffer::attachRenderBuffer(GLsizei width, GLsizei height)
{
    bind();

    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error{ std::string{"ZFX::FrameBuffer::attachRenderBuffer - glCheckFramebufferStatus failed"} };
    }

    bindDefault();
}

void ZFX::FrameBuffer::bindTextureBuffer()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureBuffer);
}

void ZFX::FrameBuffer::clear()
{
    clear(0.0f, 0.0f, 0.0f, 1.0f);
}

void ZFX::FrameBuffer::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ZFX::FrameBuffer::clear(glm::vec4 bgColour)
{
    clear(bgColour.r, bgColour.g, bgColour.b, bgColour.a);
}
