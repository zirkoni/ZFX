#pragma once
#include "Demo6A_BasicLighting.h"
#include <vector>

class Demo6B : public Demo
{
public:
    Demo6B(ZFX::Window& window) : Demo{ window, "Demo6B - Materials" }
    {
        addCubes();
        m_shinyCube.transform().scale() = glm::vec3{ 0.5f };
        m_dimCube.transform().scale() = glm::vec3{ 0.5f };

        m_shinyCube.transform().position().x = 0.7f;
        m_dimCube.transform().position().x = -0.7f;

        addLight();
        updateLighting( glm::vec3{ 1.0f } );
    }

    void draw() override
    {
        m_shinyCube.transform().rotation().z = m_counter;
        m_shinyCube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        m_light.transform().position().x = sin(50 * m_counter);

        glm::vec3 lightColour;
        lightColour.r = 1.0f - sin(m_counter);
        lightColour.g = 1.0f - cos(m_counter);
        lightColour.b = 1.0f - sin(m_counter);
        updateLighting(lightColour);

        m_light.draw(m_camera);
        m_shinyCube.draw(m_camera);
        m_dimCube.draw(m_camera);
    }

private:
    void addCubes()
    {
        m_shinyCube.load(cubeVertecesWithNormals(), cubeIndeces(), SHADERS_PATH + "colour3D_Materials");
        m_shinyCube.shader().bind();
        m_shinyCube.shader().setUniform("u_material.ambient",  glm::vec3{ 0.25f, 0.25f, 0.25f });
        m_shinyCube.shader().setUniform("u_material.diffuse",  glm::vec3{ 0.4f, 0.4f, 0.4f });
        m_shinyCube.shader().setUniform("u_material.specular", glm::vec3{ 0.774597f, 0.774597f, 0.774597f });
        m_shinyCube.shader().setUniform("u_material.shininess", 0.6f * 128);

        // We could duplicate the shiny cube but then both cubes would have the same material
        m_dimCube.load(cubeVertecesWithNormals(), cubeIndeces(), SHADERS_PATH + "colour3D_Materials");
        m_dimCube.shader().bind();
        m_dimCube.shader().setUniform("u_material.ambient",  glm::vec3{ 0.02f, 0.02f, 0.02f });
        m_dimCube.shader().setUniform("u_material.diffuse",  glm::vec3{ 0.01f, 0.01f, 0.01f });
        m_dimCube.shader().setUniform("u_material.specular", glm::vec3{ 0.4f, 0.4f, 0.4f });
        m_dimCube.shader().setUniform("u_material.shininess", 0.078125f * 128);
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

    void updateLighting(const glm::vec3& colour)
    {
        // Different components have different intensities
        m_shinyCube.shader().bind();
        m_shinyCube.shader().setUniform("u_viewPosition", m_camera.position());
        m_shinyCube.shader().setUniform("u_light.position", m_light.transform().position());
        m_shinyCube.shader().setUniform("u_light.ambient",  colour * 0.2f);
        m_shinyCube.shader().setUniform("u_light.diffuse",  colour * 0.5f);
        m_shinyCube.shader().setUniform("u_light.specular", colour * 1.0f);

        m_dimCube.shader().bind();
        m_dimCube.shader().setUniform("u_viewPosition", m_camera.position());
        m_dimCube.shader().setUniform("u_light.position", m_light.transform().position());
        m_dimCube.shader().setUniform("u_light.ambient",  colour * 0.2f);
        m_dimCube.shader().setUniform("u_light.diffuse",  colour * 0.5f);
        m_dimCube.shader().setUniform("u_light.specular", colour * 1.0f);

        // Also set the light object colour
        m_light.shader().bind();
        m_light.shader().setUniform("u_colour", glm::vec4{ colour, 1.0f } );
    }

private:
    ZFX::Object m_shinyCube;
    ZFX::Object m_dimCube;
    ZFX::Object m_light;
};
