#pragma once
#include "Demo.h"


class Demo4 : public Demo
{
public:
    Demo4(ZFX::Camera& camera) : Demo{ camera }
    {
        m_textureCircle = addTexturedCircle();
        m_textureCircle->transform.position().x = -0.2f;
        m_textureCircle->transform.scale() = glm::vec3{ 0.3f };

        m_shaderCircle = addShaderCircle();
        m_shaderCircle->transform.position().x = 0.2f;
        m_shaderCircle->transform.scale() = glm::vec3{ 0.3f };
    }

    void draw() override
    {
        m_textureCircle->draw(m_camera);
        m_shaderCircle->draw(m_camera);
    }

private:
    std::unique_ptr<TexturedShape> addTexturedCircle()
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

            ZFX::VertexAttributes{ {"positionIn", 2}, {"texCoordIn", 2} }
        };

        ZFX::Indeces indeces =
        {
            0, 2, 1,
            0, 3, 2
        };

        return std::make_unique<TexturedShape>(vertices, indeces, "circle.png");
    }

    std::unique_ptr<BasicShape> addShaderCircle()
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

            ZFX::VertexAttributes{ {"positionIn", 2} }
        };

        ZFX::Indeces indeces =
        {
            0, 2, 1,
            0, 3, 2
        };

        // Save uniform locations in Shader object
        const std::string colourUniform = "colour";
        const ZFX::Uniforms uniforms = { ZFX::TRANSFORM_UNIFORM, colourUniform };
        auto circle = std::make_unique<BasicShape>(vertices, indeces, uniforms, "circle");

        // Set colour
        circle->shader.setUniformVec4(colourUniform, ZFX::GREEN);

        return circle;
    }

private:
    std::unique_ptr<TexturedShape> m_textureCircle;
    std::unique_ptr<BasicShape> m_shaderCircle;
};
