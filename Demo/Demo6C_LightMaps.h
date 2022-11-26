#pragma once
#include "Demo6A_BasicLighting.h"
#include <vector>


ZFX::Verteces cubeVertecesWithNormalsAndTexture();

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
        m_cube.load(cubeVertecesWithNormalsAndTexture(), cubeIndeces(), SHADERS_PATH + "colour3D_LightMap");
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
        m_light.load(simpleCubeVerteces(), simpleCubeIndeces(), SHADERS_PATH + "colour3D");
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

ZFX::Verteces cubeVertecesWithNormalsAndTexture()
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

    return vertices;
}


