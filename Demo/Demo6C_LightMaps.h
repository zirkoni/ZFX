#pragma once
#include "Demo.h"
#include <vector>

class Demo6C : public Demo
{
public:
    Demo6C(ZFX::Camera& camera) : Demo{ camera, "Demo6C" }
    {
        addCube();
        m_cube.transform().scale() = glm::vec3{ 0.5f };

        addLight();
        setLightColour( glm::vec3{ 1.0f } );
    }

    void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        m_light.transform().position().x = sin(50 * m_counter);

        m_cube.shader().bind();
        m_cube.shader().setUniformVec3("u_viewPosition", m_camera.position());
        m_cube.shader().setUniformVec3("u_light.position", m_light.transform().position());

        m_light.draw(m_camera);
        m_cube.draw(m_camera);
    }

private:
    void addCube()
    {
        /* Duplicate some vertices to get clearly defined edges:
        * - each corner has a normal vector in every cardinal (x,y,z) direction
        * - 8 corners and 3 axis => 24 vertices
        */
        ZFX::Verteces vertices =
        {
            ZFX::VertexData
            {
            //   position x,y,z         normal x,y,z        texture coordinates x,y

                // +Y SIDE
                -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
                 1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
                -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
                 1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,

                // -Y SIDE
                -1.0f,  -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
                 1.0f,  -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
                -1.0f,  -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
                 1.0f,  -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,

                // +X SIDE
                1.0f,  1.0f,  1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
                1.0f,  1.0f, -1.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
                1.0f, -1.0f,  1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
                1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

                // -X SIDE
                -1.0f,  1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
                -1.0f,  1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
                -1.0f, -1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                -1.0f, -1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

                // +Z SIDE
                -1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
                 1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
                 1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

                // -Z SIDE
                -1.0f,  1.0f, -1.0f,    0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
                 1.0f,  1.0f, -1.0f,    0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
                 1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,  1.0f, 0.0f
            },

            ZFX::AttributeSizes{3, 3, 2}
        };

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

        m_cube.load(vertices, indeces, SHADERS_PATH + "colour3D_LightMap");
        m_cube.loadTexture(TEXTURES_PATH + "container.png");
        m_cube.loadTexture(TEXTURES_PATH + "container_specular.png");
        
        m_cube.shader().bind();
        m_cube.shader().setUniformVec3("u_material.ambient",  glm::vec3{ 0.25f, 0.25f, 0.25f });
        m_cube.shader().setUniformInt("u_material.diffuse",  0);
        m_cube.shader().setUniformInt("u_material.specular", 1);
        m_cube.shader().setUniformFloat("u_material.shininess", 0.6f * 128);
    }

    void addLight()
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

        m_light.load(vertices, indeces, SHADERS_PATH + "colour3D");
        m_light.shader().bind();
        m_light.shader().setUniformVec4("u_colour", glm::vec4{ 1.0f });
        m_light.transform().scale() = glm::vec3{ 0.01f };
        m_light.transform().position().x = 0.0f;
        m_light.transform().position().y = 0.0f;
        m_light.transform().position().z = 0.9f;
    }

    void setLightColour(const glm::vec3& colour)
    {
        // Different components have different intensities
        m_cube.shader().bind();
        m_cube.shader().setUniformVec3("u_light.ambient",  colour * glm::vec3{ 0.2f });
        m_cube.shader().setUniformVec3("u_light.diffuse",  colour * glm::vec3{ 0.5f });
        m_cube.shader().setUniformVec3("u_light.specular", colour * glm::vec3{ 1.0f });

        // Also set the light object colour
        m_light.shader().bind();
        m_light.shader().setUniformVec4("u_colour", glm::vec4{ colour, 1.0f } );
    }

private:
    ZFX::Object m_cube;
    ZFX::Object m_light;
};
