#pragma once
#include "Demo.hpp"


class Demo2 : public Demo
{
public:
    Demo2(ZFX::Camera& camera) : Demo{ camera }
    {
        m_square = addSquare();
    }

    void draw() override
    {
        m_square->transform.rotation().z = m_counter;
        m_counter += 0.001f;

        m_square->draw(m_camera);
    }

private:
    std::unique_ptr<BasicShape> addSquare()
    {
        /* The 4 corners of a square, starting from left lower corner in clockwise order */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //    x      y      red  green  blue  alpha
                -0.5f, -0.5f,   0.2f, 0.2f, 0.2f, 1.0f,
                -0.5f,  0.5f,   0.2f, 0.2f, 0.2f, 1.0f,
                 0.5f,  0.5f,   0.2f, 0.2f, 0.2f, 1.0f,
                 0.5f, -0.5f,   0.8f, 0.8f, 0.8f, 1.0f
            },

            ZFX::VertexAttributes{ {"positionIn", 2}, {"colourIn", 4} }
        };

        /* Now we use indeces to define our shape (square is made of 2 triangles that have 2 common verteces)
         * and set counter clockwise winding.
         */
        ZFX::Indeces indeces =
        {
            0, 2, 1, // 1st triangle: [-0.5, -0.5], [0.5f, 0.5f], [-0.5f, 0.5f]
            0, 3, 2  // 2nd triangle: [-0.5, -0.5], [0.5f, -0.5f], [0.5f, 0.5f]
        };

        return std::make_unique<BasicShape>(vertices, indeces);
    }

private:
    float m_counter;
    std::unique_ptr<BasicShape> m_square;
};
