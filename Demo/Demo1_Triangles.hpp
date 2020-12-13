#pragma once
#include "Demo.hpp"
#include <vector>


class Demo1 : public Demo
{
public:
    Demo1(ZFX::Camera& camera) : Demo{ camera }
    {
        m_triangles.push_back(std::move(addTriangle()));
        m_triangles.push_back(std::move(addTexturedTriangle()));
        m_triangles.push_back(std::move(addTexturedTriangleWithColour()));
    }

    void draw() override
    {
        m_triangles[0]->transform.position().x = sin(m_counter);
        m_triangles[1]->transform.position().y = sin(m_counter);
        m_counter += 0.001f;

        /* Draw opaque triangle 1st */
        m_triangles[1]->draw(m_camera);

        m_triangles[0]->draw(m_camera);
        m_triangles[2]->draw(m_camera);
    }

private:
    std::unique_ptr<BasicShape> addTriangle()
    {
        /* The 3 corners of a triangle (clockwise) */
#if 1
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y      red  green  blue  alpha
                -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.1f,
                 0.0f,  0.5f,   0.0f, 1.0f, 0.0f, 0.5f,
                 0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f
            },

            ZFX::VertexAttributes{ {"positionIn", 2}, {"colourIn", 4} }
        };
#else
        /* Counter clockwise => Window.cpp sets clockwise winding => this triangle is not visible */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y      red  green  blue  alpha
                -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.1f,
                 0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f,
                 0.0f,  0.5f,   0.0f, 1.0f, 0.0f, 0.5f
            },

            ZFX::VertexAttributes{ {"positionIn", 2}, {"colourIn", 4} }
        };
#endif

        /* For a triangle these don't really matter. Check out square! */
        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        return std::make_unique<BasicShape>(vertices, indeces);
    }

    std::unique_ptr<TexturedShape> addTexturedTriangle()
    {
        /* The 3 corners of a triangle */
        /* Texture coordinates have (0, 0) at the lower left corder and (1, 1) at the upper right corner */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y      texture coordinates
                -0.5f, -0.5f,     0.0f, 0.0f,
                 0.0f,  0.5f,     0.0f, 0.0f,
                 0.5f, -0.5f,     1.0f, 0.0f
            },

            ZFX::VertexAttributes{ {"positionIn", 2}, {"texCoordIn", 2} }
        };

        /* For a triangle these don't really matter. Check out square! */
        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        return std::make_unique<TexturedShape>(vertices, indeces);
    }

    std::unique_ptr<TexturedShape> addTexturedTriangleWithColour()
    {
        /* The 3 corners of a triangle */
        /* Texture coordinates have (0, 0) at the lower left corder and (1, 1) at the upper right corner */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y     red  green  blue  alpha   texture coordinates
               -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.8f,     0.0f, 0.0f,
                0.0f,  0.5f,   0.0f, 1.0f, 0.0f, 0.8f,     0.0f, 0.0f,
                0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 0.8f,     1.0f, 0.0f
            },

            ZFX::VertexAttributes{ {"positionIn", 2}, {"colourIn", 4}, {"texCoordIn", 2} }
        };

        /* For a triangle these don't really matter. Check out square! */
        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        return std::make_unique<TexturedShape>(vertices, indeces, "textureAndColour");
    }

private:
    float m_counter;
    std::vector<std::unique_ptr<BasicShape> > m_triangles;
};
