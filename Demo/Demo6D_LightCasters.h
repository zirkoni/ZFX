#pragma once
#include "Demo6C_LightMaps.h"
#include <vector>

class Demo6D : public Demo
{
public:
    Demo6D(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo6D - Multiple Lights" }
    {
        addCube();
        m_cube.transform().scale() = glm::vec3{ 0.5f };
        m_cube.transform().position().x = 0.7f;
        m_cube.duplicate().position().x = -0.7f;
        addDirectionalLight();
        addPointLights();
        addSpotLight();
    }

    virtual ~Demo6D() {}

    virtual void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        // Spotlight always comes 'out-of-camera'
        m_spotLight->bind();
        m_spotLight->setPosition(m_camera.position());
        m_spotLight->setDirection(m_camera.front());

        m_cube.shader().bind();
        m_cube.shader().setUniform("u_viewPosition", m_camera.position());
        m_cube.draw(m_camera);

        for(auto& light : m_pointLights)
        {
            light.draw(m_camera);
        }
    }

protected:
    void addCube()
    {
        m_cube.load(cubeVertecesWithNormalsAndTexture(), cubeIndeces(), SHADERS_PATH + "colour3D_LightCasters");
        m_cube.loadTexture(TEXTURES_PATH + "container.png");
        m_cube.loadTexture(TEXTURES_PATH + "container_specular.png");

        m_cube.shader().bind();
        m_cube.shader().setUniform("u_material.diffuse",  0);
        m_cube.shader().setUniform("u_material.specular", 1);
        m_cube.shader().setUniform("u_material.shininess", 0.6f * 128);
    }

    void addDirectionalLight()
    {
        // Local variable, only create and set once
        ZFX::DirectionalLight dirLight{ "u_dirLight", m_cube.shader() };
        dirLight.bind();
        dirLight.setDirection( glm::vec3{-0.2f, -1.0f, -0.3f} );
        dirLight.setAmbient( glm::vec3{0.05f} );
        dirLight.setDiffuse( glm::vec3{0.4f} );
        dirLight.setSpecular( glm::vec3{0.5f} );
    }

    void addPointLights()
    {
        // Set 4 point lights (same number as in shader)

        glm::vec3 pointLightPositions[] =
        {
            glm::vec3{  0.7f,  0.2f,  2.0f },
            glm::vec3{  2.3f, -3.3f, -4.0f },
            glm::vec3{ -2.0f,  2.0f, -2.0f },
            glm::vec3{  0.0f,  0.0f, -3.0f }
        };

        glm::vec3 pointLightColours[] =
        {
            glm::vec3{ 1.0f, 0.0f, 0.0f },
            glm::vec3{ 0.0f, 1.0f, 0.0f },
            glm::vec3{ 0.0f, 0.0f, 1.0f },
            glm::vec3{ 1.0f, 1.0f, 0.0f }
        };

        for(int i = 0; i < 4; ++i)
        {
            ZFX::PointLight pLight{ "u_pointLights[" + std::to_string(i) + "]", m_cube.shader() };
            pLight.bind(); // Bind called for the same shader every loop...
            pLight.setPosition(pointLightPositions[i]);
            pLight.setAmbient( pointLightColours[i] * glm::vec3{0.05f} );
            pLight.setDiffuse( pointLightColours[i] * glm::vec3{0.8f} );
            pLight.setSpecular( pointLightColours[i] * glm::vec3{1.0f} );
            pLight.setConstant(1.0f);
            pLight.setLinear(0.09f);
            pLight.setQuadratic(0.032f);

            // All of these use the same data & shader => could be optimized but different colour...
            addPointLightObject(pointLightPositions[i], pointLightColours[i]);
        }
    }

    void addPointLightObject(const glm::vec3& position, const glm::vec3& colour)
    {
        ZFX::Object cube;
        cube.load(simpleCubeVerteces(), simpleCubeIndeces(), SHADERS_PATH + "colour3D");
        cube.shader().bind();
        cube.shader().setUniform("u_colour", glm::vec4{ colour, 1.0f });
        cube.transform().scale() = glm::vec3{ 0.05f };
        cube.transform().position() = position;

        m_pointLights.push_back(std::move(cube));
    }

    void addSpotLight()
    {
        m_spotLight = std::make_unique<ZFX::SpotLight>("u_spotLight", m_cube.shader());
        m_spotLight->bind();
        m_spotLight->setPosition(m_camera.position());
        m_spotLight->setDirection(m_camera.front());
        m_spotLight->setAmbient( glm::vec3{0.0f} );
        m_spotLight->setDiffuse( glm::vec3{1.0f} );
        m_spotLight->setSpecular( glm::vec3{1.0f} );
        m_spotLight->setConstant(1.0f);
        m_spotLight->setLinear(0.09f);
        m_spotLight->setQuadratic(0.032f);
        m_spotLight->setCutOff(glm::cos(glm::radians(12.5f)));
        m_spotLight->setOuterCutOff(glm::cos(glm::radians(15.0f)));
    }

protected:
    ZFX::Object m_cube;
    std::vector<ZFX::Object> m_pointLights;
    std::unique_ptr<ZFX::SpotLight> m_spotLight;
};
