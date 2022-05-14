#pragma once
#include "Demo.h"
#include <vector>


class Demo1 : public Demo
{
    static constexpr uint32_t IDX_X_MOVE = 0;
    static constexpr uint32_t IDX_Y_MOVE = 1;

public:
    Demo1(ZFX::Camera& camera) : Demo{ camera }
    {
        m_triangles.push_back(std::move(addTriangle()));
        m_triangles.push_back(std::move(addTexturedTriangle()));
        m_triangles.push_back(std::move(addTexturedTriangleWithColour()));

        /* Triangles with a single solid colour */
        m_triangles.push_back(std::move(addTriangle( ZFX::RED,    glm::vec2{ -1.0f,  1.0f }, 0.1f )));
        m_triangles.push_back(std::move(addTriangle( ZFX::GREEN,  glm::vec2{  1.0f,  1.0f }, 0.2f )));
        m_triangles.push_back(std::move(addTriangle( ZFX::BLUE,   glm::vec2{ -1.0f, -1.0f }, 0.3f )));
        m_triangles.push_back(std::move(addTriangle( ZFX::YELLOW, glm::vec2{  1.0f, -1.0f }, 0.5f)));
    }

    void draw() override
    {
        m_triangles.at(IDX_X_MOVE)->transform.position().x = sin(m_counter);
        m_triangles.at(IDX_Y_MOVE)->transform.position().y = sin(m_counter);
        m_counter += 0.001f;

        for (auto& t : m_triangles)
        {
            t->draw(m_camera);
        }
    }

private:
    std::unique_ptr<BasicShape> addTriangle()
    {
        /* The 3 corners of a triangle (clockwise) */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y      red  green  blue  alpha
                -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.1f,
                 0.0f,  0.5f,   0.0f, 1.0f, 0.0f, 0.5f,
                 0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f
            },

            ZFX::AttributeSizes{2, 4} // 2 x float for coordinates, 4 x float for colour
        };

        /* With indeces we must choose the vertices in counter clockwise order
         * because we are culling back faces and by default OpenGL winding order is counter clockwise.
         */
        ZFX::Indeces indeces =
        {
            0, 2, 1
        };

        return std::make_unique<BasicShape>(vertices, indeces);
    }

    std::unique_ptr<BasicShape> addTriangle(const glm::vec4& colour, const glm::vec2& position, float scale)
    {
        /* The 3 corners of a triangle (counter clockwise) */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y
                -0.5f, -0.5f,
                 0.5f, -0.5f,
                 0.0f,  0.5f,
            },

            ZFX::AttributeSizes{2}
        };

        /* Now indeces are different! */
        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        // Save uniform locations in Shader object
        const std::string colourUniform = "colour";
        const ZFX::Uniforms uniforms = { ZFX::MODEL_UNIFORM, ZFX::VIEW_PROJECTION_UNIFORM, colourUniform };
        auto triangle = std::make_unique<BasicShape>(vertices, indeces, uniforms);

        // Set colour
        triangle->shader.setUniformVec4(colourUniform, colour);

        // set location
        triangle->transform.position() = glm::vec3{ position.x, position.y, 0.0f };

        // set size
        triangle->transform.scale() = glm::vec3{ scale };

        return triangle;
    }

    std::unique_ptr<TexturedShape> addTexturedTriangle()
    {
        /* Texture coordinates have (0, 0) at the lower left corner and (1, 1) at the upper right corner */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y      texture coordinates
                -0.5f, -0.5f,     0.0f, 0.0f,
                 0.5f, -0.5f,     1.0f, 0.0f,
                 0.0f,  0.5f,     0.5f, 1.0f
            },

            ZFX::AttributeSizes{2, 2}
        };

        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        return std::make_unique<TexturedShape>(vertices, indeces, "texture.png");
    }

    std::unique_ptr<TexturedShape> addTexturedTriangleWithColour()
    {
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y     red  green  blue  alpha   texture coordinates
               -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.8f,     0.0f, 0.0f,
                0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 0.8f,     1.0f, 0.0f,
                0.0f,  0.5f,   0.0f, 1.0f, 0.0f, 0.8f,     0.5f, 1.0f,
            },

            ZFX::AttributeSizes{2, 4, 2}
        };

        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        return std::make_unique<TexturedShape>(vertices, indeces, "texture.png", "textureAndColour");
    }

private:
    float m_counter;
    std::vector<std::unique_ptr<BasicShape> > m_triangles;
};
