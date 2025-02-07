#include "FrameBuffer.h"
#include "Window.h"
#include "zfxdefs.h"
#include <string>


ZFX::FrameBuffer::FrameBuffer(int numMSAASamples, uint32_t bitsToClear):
    m_fbo{0}, m_textureBuffer{0}, m_rbo{0}, m_depthBuffer{0}, m_numMSAASamples{numMSAASamples}, m_bitsToClear{bitsToClear}
{
    glGenFramebuffers(1, &m_fbo);
}

ZFX::FrameBuffer::~FrameBuffer()
{
    deleteResources();
}

void ZFX::FrameBuffer::deleteResources()
{
    if(m_hasTextureBuffer)
    {
        glDeleteTextures(1, &m_textureBuffer);
        m_hasTextureBuffer = false;
    }

    if(m_hasRbo)
    {
        glDeleteRenderbuffers(1, &m_rbo);
        m_hasRbo = false;
    }

    if(m_hasDepthBuffer)
    {
        glDeleteTextures(1, &m_depthBuffer);
        m_hasDepthBuffer = false;
    }

    glDeleteFramebuffers(1, &m_fbo);
}

void ZFX::FrameBuffer::reset(int numMSAASamples)
{
    deleteResources();
    m_numMSAASamples = numMSAASamples;
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
    if(!m_hasTextureBuffer)
    {
        bind();

        glGenTextures(1, &m_textureBuffer);

        if(m_numMSAASamples > 0)
        {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureBuffer);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_numMSAASamples, GL_RGBA, width, height, GL_TRUE);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_textureBuffer, 0);
        } else
        {
            glBindTexture(GL_TEXTURE_2D, m_textureBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureBuffer, 0);
        }

        bindDefault();
        m_hasTextureBuffer = true;
    } else
    {
        throw ZFX::Exception{__FILE__, __LINE__, "ZFX::FrameBuffer::attachTextureBuffer - already attached"};
    }
}

void ZFX::FrameBuffer::attachRenderBuffer(GLsizei width, GLsizei height)
{
    if(!m_hasRbo)
    {
        bind();

        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

        if(m_numMSAASamples > 0)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_numMSAASamples, GL_DEPTH24_STENCIL8, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        } else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw ZFX::Exception{__FILE__, __LINE__, "ZFX::FrameBuffer::attachRenderBuffer - glCheckFramebufferStatus failed"};
        }

        bindDefault();
        m_hasRbo = true;
    } else
    {
        throw ZFX::Exception{__FILE__, __LINE__, "ZFX::FrameBuffer::attachRenderBuffer - already attached"};
    }
}

void ZFX::FrameBuffer::attachDepthBuffer(GLsizei width, GLsizei height)
{
    if(!m_hasDepthBuffer)
    {
        // create depth texture
        glGenTextures(1, &m_depthBuffer);
        glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        bindDefault();
        m_hasDepthBuffer = true;
    } else
    {
        throw ZFX::Exception{__FILE__, __LINE__, "ZFX::FrameBuffer::attachDepthBuffer - already attached"};
    }
}

void ZFX::FrameBuffer::attachDepthCubeMap(GLsizei width, GLsizei height)
{
    if(!m_hasDepthBuffer)
    {
        // create cube depth texture
        glGenTextures(1, &m_depthBuffer);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthBuffer);
        for(int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // attach depth texture as FBO's depth buffer
        bind();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthBuffer, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        bindDefault();
        m_hasDepthBuffer = true;
    } else
    {
        throw ZFX::Exception{__FILE__, __LINE__, "ZFX::FrameBuffer::attachDepthCubeMap - already attached"};
    }
}

void ZFX::FrameBuffer::bindTextureBuffer()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureBuffer);
}

void ZFX::FrameBuffer::bindDepthBuffer()
{
    glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
}

void ZFX::FrameBuffer::bindDepthCubeMap()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthBuffer);
}

void ZFX::FrameBuffer::clear()
{
    clear(0.0f, 0.0f, 0.0f, 1.0f);
}

void ZFX::FrameBuffer::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(m_bitsToClear);
}

void ZFX::FrameBuffer::clear(glm::vec4 bgColour)
{
    clear(bgColour.r, bgColour.g, bgColour.b, bgColour.a);
}
