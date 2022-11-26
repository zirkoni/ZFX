#pragma once
#include "Demo6D_LightCasters.h"
#include <vector>

class Demo6E : public Demo6D
{
public:
    Demo6E(ZFX::Camera& camera, ZFX::Window& window) : Demo6D{camera}, m_activeEffect{4}
    {
        m_name = "Demo6E";
        addFrameBuffer(window);
    }

    void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        m_buffer.bind(); // draw to frame buffer
        m_buffer.clear(bgColour());

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

        drawFrameBufferToScreen();
    }

    void onExit() override
    {
        glEnable(GL_DEPTH_TEST);
    }

    void handleInput(const SDL_Event& e) override
    {
        if(e.key.keysym.scancode == SDL_SCANCODE_D)
        {
            ++m_activeEffect;
            if(m_activeEffect > 4)
            {
                m_activeEffect = 0;
            }

            m_screen.shader().bind();
            m_screen.shader().setUniformInt("u_selected", m_activeEffect);
        }
    }

private:

    void addFrameBuffer(ZFX::Window& window)
    {
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //   position x,y   texture x,y
                -1.0f, -1.0f,   0.0f, 0.0f,
                -1.0f,  1.0f,   0.0f, 1.0f,
                 1.0f,  1.0f,   1.0f, 1.0f,
                 1.0f, -1.0f,   1.0f, 0.0f
            },

            ZFX::AttributeSizes{2, 2}
        };

        ZFX::Indeces indeces = { 0, 2, 1, 0, 3, 2 };

        m_screen.load(vertices, indeces, SHADERS_PATH + "framebuffer");
        m_screen.shader().bind();
        m_screen.shader().setUniformInt("u_screenTexture", 0);

        m_buffer.attachTextureBuffer(window.width(), window.height());
        m_buffer.attachRenderBuffer(window.width(), window.height()); // For depth testing
    }

    void drawFrameBufferToScreen()
    {
        ZFX::FrameBuffer::bindDefault(); // draw to screen
        glDisable(GL_DEPTH_TEST);

        m_screen.shader().bind();
        m_buffer.bindTextureBuffer();
        m_screen.mesh().draw();
    }

private:
    ZFX::FrameBuffer m_buffer;
    ZFX::Object m_screen;
    int m_activeEffect;
};
