#pragma once
#include "Demo.h"
#include <vector>

class Demo6D : public Demo
{
public:
    Demo6D(ZFX::Camera& camera) : Demo{ camera, "Demo6D" }
    {
        addCube();
        m_cube.transform().scale() = glm::vec3{ 0.5f };
        m_cube.transform().position().x = 0.7f;
        m_cube.duplicate().position().x = -0.7f;
        addDirectionalLight();
        addPointLights();
        addSpotLight();
    }

    void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        // Spotlight always comes 'out-of-camera'
        m_spotLight->setPosition(m_camera.position());
        m_spotLight->setDirection(m_camera.front());

        m_cube.shader().setUniformVec3("viewPosition", m_camera.position());
        m_cube.draw(m_camera);

        for(auto& light : m_pointLights)
        {
            light.draw(m_camera);
        }
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

        m_cube.load(vertices, indeces, SHADERS_PATH + "colour3D_LightCasters");
        m_cube.loadTexture(TEXTURES_PATH + "container.png");
        m_cube.loadTexture(TEXTURES_PATH + "container_specular.png");
        
        m_cube.shader().setUniformVec3("material.ambient",  glm::vec3{ 0.25f, 0.25f, 0.25f });
        m_cube.shader().setUniformInt("material.diffuse",  0);
        m_cube.shader().setUniformInt("material.specular", 1);
        m_cube.shader().setUniformFloat("material.shininess", 0.6f * 128);
    }

    void addDirectionalLight()
    {
        // Local variable, only create and set once
        ZFX::DirectionalLight dirLight{ "dirLight", m_cube.shader() };
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
            ZFX::PointLight pLight{ "pointLights[" + std::to_string(i) + "]", m_cube.shader() };
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

        ZFX::Object cube;
        cube.load(vertices, indeces, SHADERS_PATH + "colour3D");
        cube.shader().setUniformVec4("colour", glm::vec4{ colour, 1.0f });
        cube.transform().scale() = glm::vec3{ 0.05f };
        cube.transform().position() = position;

        m_pointLights.push_back(std::move(cube));
    }

    void addSpotLight()
    {
        m_spotLight = std::make_unique<ZFX::SpotLight>("spotLight", m_cube.shader());
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

private:
    ZFX::Object m_cube;
    std::vector<ZFX::Object> m_pointLights;
    std::unique_ptr<ZFX::SpotLight> m_spotLight;
};
