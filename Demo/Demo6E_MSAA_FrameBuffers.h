#pragma once
#include "Demo6E_FrameBuffers.h"
#include <vector>

class Demo6E_MSAA : public Demo6E
{
public:
    Demo6E_MSAA(ZFX::Camera& camera, ZFX::Window& window) : Demo6E{camera, window}, m_MSAABuffer{4}
    {
        m_name = "Demo6E_MSAA";
        addMSAABuffer(window);
    }

    void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        m_MSAABuffer.bind(); // draw to MSAA buffer
        m_MSAABuffer.clear(bgColour());

        glEnable(GL_DEPTH_TEST);

        // Spotlight always comes 'out-of-camera'
        m_spotLight->bind();
        m_spotLight->setPosition(m_camera.position());
        m_spotLight->setDirection(m_camera.front());

        m_cube.shader().bind();
        m_cube.shader().setUniformVec3("u_viewPosition", m_camera.position());
        m_cube.draw(m_camera);

        for(auto& light : m_pointLights)
        {
            light.draw(m_camera);
        }

        // Blit MSAA buffer to framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSAABuffer.fbo());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_buffer.fbo());
        glBlitFramebuffer(0, 0, ZFX::Window::width(), ZFX::Window::height(), 0, 0, ZFX::Window::width(), ZFX::Window::height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

        drawFrameBufferToScreen();
    }

private:

    void addMSAABuffer(ZFX::Window& window)
    {
        m_MSAABuffer.attachTextureBuffer(window.width(), window.height());
        m_MSAABuffer.attachRenderBuffer(window.width(), window.height()); // For depth testing
    }

private:
    ZFX::FrameBuffer m_MSAABuffer;
};
