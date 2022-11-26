#pragma once
#include "Demo.h"


class Demo4 : public Demo
{
public:
    Demo4(ZFX::Camera& camera) : Demo{ camera, "Demo4" }
    {
        addTexturedCircle();
        m_textureCircle.transform().position().x = -0.2f;
        m_textureCircle.transform().scale() = glm::vec3{ 0.3f };

        addShaderCircle();
        m_shaderCircle.transform().position().x = 0.2f;
        m_shaderCircle.transform().scale() = glm::vec3{ 0.3f };
    }

    void draw() override
    {
        m_textureCircle.draw(m_camera);
        m_shaderCircle.draw(m_camera);
    }

private:
    void addTexturedCircle()
    {
        /* Create a square with texture that is a circle */

        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
                -0.5f, -0.5f,   0.0f, 0.0f,
                -0.5f,  0.5f,   0.0f, 1.0f,
                 0.5f,  0.5f,   1.0f, 1.0f,
                 0.5f, -0.5f,   1.0f, 0.0f
            },

            ZFX::AttributeSizes{2, 2}
        };

        ZFX::Indeces indeces =
        {
            0, 2, 1,
            0, 3, 2
        };

        m_textureCircle.load(vertices, indeces, SHADERS_PATH + "texture");
        m_textureCircle.loadTexture(TEXTURES_PATH + "circle.png");
    }

    void addShaderCircle()
    {
        /* Create a square, fragment shader will draw it as a circle */

        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
                -0.5f, -0.5f,
                -0.5f,  0.5f,
                 0.5f,  0.5f,
                 0.5f, -0.5f
            },

            ZFX::AttributeSizes{2}
        };

        ZFX::Indeces indeces =
        {
            0, 2, 1,
            0, 3, 2
        };

        m_shaderCircle.load(vertices, indeces, SHADERS_PATH + "circle");

        // Set colour
        m_shaderCircle.shader().bind();
        m_shaderCircle.shader().setUniformVec4("u_colour", ZFX::GREEN);
    }

private:
    ZFX::Object m_textureCircle;
    ZFX::Object m_shaderCircle;
};
