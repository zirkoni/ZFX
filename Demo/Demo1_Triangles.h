#pragma once
#include "Demo.h"
#include <vector>


class Demo1 : public Demo
{
    static constexpr uint32_t IDX_X_MOVE = 0;
    static constexpr uint32_t IDX_Y_MOVE = 1;

public:
    Demo1(ZFX::Camera& camera) : Demo{ camera, "Demo1" }
    {
        addTriangle();
        addTexturedTriangle();
        addTexturedTriangleWithColour();
        addSmallTriangles();
    }

    void draw() override
    {
        m_triangles.at(IDX_X_MOVE).transform().position().x = sin(m_counter);
        m_triangles.at(IDX_Y_MOVE).transform().position().y = sin(m_counter);
        m_counter += 0.001f;

        for (auto& t : m_triangles)
        {
            t.draw(m_camera);
        }
    }

private:
    void addTriangle()
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

        ZFX::Object triangle;
        triangle.load(vertices, indeces, SHADERS_PATH + "colour");
        m_triangles.push_back(std::move(triangle));
    }

    void addSmallTriangles()
    {
        /* Triangles with a single solid colour
         * All of these use the same shader => no need to duplicate vertex data or shader
         * */
        glm::vec2 positions[] =
        {
            glm::vec2{ -1.0f,  1.0f },
            glm::vec2{  1.0f,  1.0f },
            glm::vec2{ -1.0f, -1.0f },
            glm::vec2{  1.0f, -1.0f }
        };

        float sizes[] = { 0.1f, 0.2f, 0.3f, 0.5f };

        loadTriangle(positions[0], sizes[0]);

        for(int i = 1; i < 4; ++i)
        {
            auto& transform = m_triangles.back().duplicate();
            transform.position() = glm::vec3{ positions[i], 0.0f };
            transform.scale() = glm::vec3{ sizes[i] };
        }
    }

    void loadTriangle(const glm::vec2& position, float scale)
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

        ZFX::Object triangle;
        triangle.load(vertices, indeces, SHADERS_PATH + "colour2");
        triangle.transform().position() = glm::vec3{ position, 0.0f };
        triangle.transform().scale() = glm::vec3{ scale };
        triangle.shader().setUniformVec4("colour", ZFX::YELLOW);
        m_triangles.push_back(std::move(triangle));
    }

    void addTexturedTriangle()
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

        ZFX::Object triangle;
        triangle.load(vertices, indeces, SHADERS_PATH + "texture");
        triangle.loadTexture(TEXTURES_PATH + "texture.png");
        m_triangles.push_back(std::move(triangle));
    }

    void addTexturedTriangleWithColour()
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

        ZFX::Object triangle;
        triangle.load(vertices, indeces, SHADERS_PATH + "textureAndColour");
        triangle.loadTexture(TEXTURES_PATH + "texture.png");
        m_triangles.push_back(std::move(triangle));
    }

private:
    std::vector<ZFX::Object> m_triangles;
};
