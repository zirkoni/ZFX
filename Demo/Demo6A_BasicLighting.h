#pragma once
#include "Demo5_Cube.h"
#include <vector>


ZFX::Verteces cubeVertecesWithNormals();
ZFX::Indeces cubeIndeces();

class Demo6A : public Demo
{
public:
    Demo6A(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo6A - Basic Lighting" }
    {
        addCube();
        m_cube.transform().scale() = glm::vec3{ 0.5f };

        addLight();
    }

    void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        m_light.transform().position().x = sin(50 * m_counter);

        m_cube.shader().bind();
        m_cube.shader().setUniform("u_viewPosition", m_camera.position());
        m_cube.shader().setUniform("u_lightPosition", m_light.transform().position());

        m_light.draw(m_camera);
        m_cube.draw(m_camera);
    }

private:
    void addCube()
    {
        m_cube.load(cubeVertecesWithNormals(), cubeIndeces(), SHADERS_PATH + "colour3D_Lighting");
    }

    void addLight()
    {
        m_light.load(simpleCubeVerteces(), simpleCubeIndeces(), SHADERS_PATH + "colour3D");

        m_light.shader().bind();
        m_light.shader().setUniform("u_colour", ZFX::WHITE);
        m_light.transform().scale() = glm::vec3{ 0.01f };
        m_light.transform().position().x = 0.0f;
        m_light.transform().position().y = 0.0f;
        m_light.transform().position().z = 0.9f;
    }

private:
    ZFX::Object m_cube;
    ZFX::Object m_light;
};

ZFX::Verteces cubeVertecesWithNormals()
{
    /* Duplicate some vertices to get clearly defined edges:
    * - each corner has a normal vector in every cardinal (x,y,z) direction
    * - 8 corners and 3 axis => 24 vertices
    */
    ZFX::Verteces vertices =
    {
        ZFX::VertexData
        {
        //   position x,y,z         normal x,y,z

            // +Y SIDE
            -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,

            // -Y SIDE
            -1.0f,  -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
             1.0f,  -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
            -1.0f,  -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,
             1.0f,  -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,

            // +X SIDE
            1.0f,  1.0f,  1.0f,     1.0f, 0.0f, 0.0f,
            1.0f,  1.0f, -1.0f,     1.0f, 0.0f, 0.0f,
            1.0f, -1.0f,  1.0f,     1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 0.0f,

            // -X SIDE
            -1.0f,  1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,

            // +Z SIDE
            -1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
             1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,

            // -Z SIDE
            -1.0f,  1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
             1.0f,  1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
             1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f
        },

        ZFX::AttributeSizes{3, 3}
    };

    return vertices;
}

ZFX::Indeces cubeIndeces()
{
    ZFX::Indeces indeces =
    {
        // +X
        8, 10, 9, 9, 10, 11,

        // -X
        14, 12, 13, 14, 13, 15,

        // +Y
        1, 0, 2, 3, 1, 2,

        // -Y
        4, 5, 6, 5, 7, 6,

        // +Z
        17, 16, 18, 19, 17, 18,

        // -Z
        20, 21, 22, 21, 23, 22
    };

    return indeces;
}

