#pragma once
#include "Demo.h"
#include <vector>

class Demo5 : public Demo
{
public:
    Demo5(ZFX::Camera& camera) : Demo{ camera }
    {
        m_cube = addCube();
        m_cube->transform.scale() = glm::vec3{ 0.5f };
    }

    void draw() override
    {
        m_cube->transform.rotation().z = m_counter;
        m_cube->transform.rotation().x = m_counter;
        m_counter += 0.001f;

        m_cube->draw(m_camera);
    }

private:
    std::unique_ptr<BasicShape> addCube()
    {
        /* The 8 corners of a cube */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
                //   x   y   z      r   g   b   a
                    -1, -1, -1,     1,  0,  0,  1, // 0 left low
                     1, -1, -1,     0,  1,  0,  1, // 1 right low
                     1,  1, -1,     0,  0,  1,  1, // 2 right high
                    -1,  1, -1,     1,  1,  0,  1, // 3 left high

                    -1, -1,  1,     1,  0,  0,  1, // 4 left low
                     1, -1,  1,     0,  1,  0,  1, // 5 right low
                     1,  1,  1,     0,  0,  1,  1, // 6 right high
                    -1,  1,  1,     1,  1,  0,  1  // 7 left high
                },

                ZFX::VertexAttributes{ {"positionIn", 3}, {"colourIn", 4} }
        };

        ZFX::Indeces indeces =
        {
            /* Back */
            0, 3, 1, 3, 2, 1,

            /* Right */
            1, 2, 5, 2, 6, 5,

            /* Front */
            5, 6, 4, 6, 7, 4,

            /* Left */
            4, 7, 0, 7, 3, 0,

            /* Top */
            3, 7, 2, 7, 6, 2,

            /* Bottom */
            4, 0, 5, 0, 1, 5
        };

        return std::make_unique<BasicShape>(vertices, indeces, "colour3D");
    }

private:
    float m_counter;
    std::unique_ptr<BasicShape> m_cube;
};
