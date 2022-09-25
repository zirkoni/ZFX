#pragma once
#include "Demo.h"
#include <vector>


// Omnidirectional shadow map
// Example is taken from: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

class Demo6G : public Demo
{
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

public:
    Demo6G(ZFX::Camera& camera, ZFX::Window& window) : Demo{ camera, "Demo6G" }, m_window{window}
    {
        loadShaders();
        loadCubes();
        loadDepthBuffer();
        m_lightPos = glm::vec3{ 0.0f };

        onEntry();
    }

    void draw() override
    {
        // move light position over time
        m_lightPos.z = sin(m_counter);
        m_counter += 0.01f;

        // 0. create depth cubemap transformation matrices
        float nearPlane = 1.0f;
        float farPlane = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, nearPlane, farPlane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        // 1. render scene to depth cubemap
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        m_depthBuffer.bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_simpleDepthShader->setUniformMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        }
        m_simpleDepthShader->setUniformFloat("farPlane", farPlane);
        m_simpleDepthShader->setUniformVec3("lightPos", m_lightPos);
        renderDepth();
        m_depthBuffer.bindDefault();

        // 2. render scene as normal
        glViewport(0, 0, m_window.width(), m_window.height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 viewProjection = m_camera.getViewProjection();
        m_shader->setUniformMat4("viewProjection", viewProjection);
        m_shader->setUniformVec3("viewPos", m_camera.position());
        m_shader->setUniformVec3("lightPos", m_lightPos);
        m_shader->setUniformFloat("farPlane", farPlane);
        glActiveTexture(GL_TEXTURE1);
        m_depthBuffer.bindDepthCubeMap();
        renderScene();
    }

    void onEntry() override
    {
        m_camera.resetZoom();
        m_camera.position() = glm::vec3{4.0f, -3.0f, -3.0f};
        m_camera.yaw() = 150.0f;
        m_camera.pitch() = 20.0f;
        m_camera.update();
    }

private:
    void loadShaders()
    {
        ZFX::ShaderSource shaderSrc = {SHADERS_PATH + "colour3D_PointShadows.vs", SHADERS_PATH + "colour3D_PointShadows.fs"};
        m_shader = std::make_shared<ZFX::Shader>(shaderSrc);

        // Vertex + fragment + geometry shader
        ZFX::ShaderSource depthSrc = {SHADERS_PATH + "depthCube.vs", SHADERS_PATH + "depthCube.fs", SHADERS_PATH + "depthCube.gs"};
        m_simpleDepthShader = std::make_unique<ZFX::Shader>(depthSrc);

        m_shader->setUniformInt("diffuseTexture", 0);
        m_shader->setUniformInt("depthMap", 1);
    }

    void loadCubes()
    {
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

        m_room.load(vertices, indeces, m_shader);
        m_room.loadTexture(TEXTURES_PATH + "texture.png");
        m_room.transform().scale() = glm::vec3{5.0f};

        m_cubes.load(vertices, indeces, m_shader);
        m_cubes.loadTexture(TEXTURES_PATH + "container.png");

        // 1st cube
        auto& cube1 = m_cubes.transform();
        cube1.scale() = glm::vec3{0.5f};
        cube1.position() = glm::vec3{4.0f, -3.5f, 0.0};

        // 2nd cube
        auto& cube2 = m_cubes.duplicate();
        cube2.scale() = glm::vec3{0.75f};
        cube2.position() = glm::vec3{2.0f, 3.0f, 1.0};

        // 3rd cube
        auto& cube3 = m_cubes.duplicate();
        cube3.scale() = glm::vec3{0.5f};
        cube3.position() = glm::vec3{-3.0f, -1.0f, 0.0};

        // 4th cube
        auto& cube4 = m_cubes.duplicate();
        cube4.scale() = glm::vec3{0.5f};
        cube4.position() = glm::vec3{-1.5f, 1.0f, 1.5};

        // 5th cube
        auto& cube5 = m_cubes.duplicate();
        cube5.scale() = glm::vec3{0.75f};
        cube5.position() = glm::vec3{-1.5f, 2.0f, -3.0};
        cube5.rotation().x = glm::radians(60.0f);
        cube5.rotation().z = glm::radians(60.0f);
    }

    void loadDepthBuffer()
    {
        m_depthBuffer.attachDepthCubeMap(SHADOW_WIDTH, SHADOW_HEIGHT);
    }

    void renderDepth()
    {
        auto& shader = *m_simpleDepthShader.get();
        m_room.draw(shader);
        m_cubes.draw(shader);
    }

    void renderScene()
    {
        // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
        glDisable(GL_CULL_FACE);
        // A small little hack to invert normals when drawing cube from the inside so lighting still works.
        m_shader->setUniformInt("reverseNormals", 1);
        m_room.draw(m_camera);
        m_shader->setUniformInt("reverseNormals", 0); // and of course disable it
        glEnable(GL_CULL_FACE);

        m_cubes.draw(m_camera);
    }

private:
    ZFX::Window& m_window;
    std::shared_ptr<ZFX::Shader> m_shader;
    std::unique_ptr<ZFX::Shader> m_simpleDepthShader;
    ZFX::Object m_room;
    ZFX::Object m_cubes;
    ZFX::FrameBuffer m_depthBuffer;
    glm::vec3 m_lightPos;
};
