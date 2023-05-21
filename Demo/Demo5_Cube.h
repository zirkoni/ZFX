#pragma once
#include "Demo.h"
#include <vector>


ZFX::Verteces simpleCubeVerteces();
ZFX::Indeces simpleCubeIndeces();

class Demo5 : public Demo
{
public:
    Demo5(ZFX::Camera& camera) : Demo{ camera, "Demo5 - Simple Cube" }
    {
        addCube();
        m_cube.transform().scale() = glm::vec3{ 0.5f };
    }

    void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        m_cube.draw(m_camera);
    }

private:
    void addCube()
    {
        m_cube.load(simpleCubeVerteces(), simpleCubeIndeces(), SHADERS_PATH + "colour3D");

//#define CHANGE_COLOUR
#if defined(CHANGE_COLOUR)
        m_cube.shader().bind();
        m_cube.shader().setUniform("u_colour", glm::vec4{ 0.0f, 0.5f, 0.0f, 1.0f });
#endif
    }

private:
    ZFX::Object m_cube;
};

ZFX::Verteces simpleCubeVerteces()
{
    /* The 8 corners of a cube */
    ZFX::Verteces vertices =
    {
        ZFX::VertexData
        {
            //   x   y   z
                -1, -1, -1, // 0 left low
                 1, -1, -1, // 1 right low
                 1,  1, -1, // 2 right high
                -1,  1, -1, // 3 left high

                -1, -1,  1, // 4 left low
                 1, -1,  1, // 5 right low
                 1,  1,  1, // 6 right high
                -1,  1,  1, // 7 left high
        },

        ZFX::AttributeSizes{3}
    };

    return vertices;
}

ZFX::Indeces simpleCubeIndeces()
{
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

    return indeces;
}


