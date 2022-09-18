#pragma once
#include "Demo.h"


class Demo3 : public Demo
{
public:
    Demo3(ZFX::Camera& camera) : Demo{ camera, "Demo3" }
    {
        addSquare();
    }

    void draw() override
    {
        m_square.transform().rotation().z = m_counter;
        m_counter += 0.001f;

        m_square.draw(m_camera);
    }

private:
    void addSquare()
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

            ZFX::AttributeSizes{2, 4}
        };

        /* Now we use indeces to define our shape (square is made of 2 triangles that have 2 common verteces)
         * and set counter clockwise winding.
         */
        ZFX::Indeces indeces =
        {
            0, 2, 1, // 1st triangle: [-0.5, -0.5], [0.5f, 0.5f], [-0.5f, 0.5f]
            0, 3, 2  // 2nd triangle: [-0.5, -0.5], [0.5f, -0.5f], [0.5f, 0.5f]
        };

        m_square.load(vertices, indeces, SHADERS_PATH + "colour");
    }

private:
    ZFX::Object m_square;
};
