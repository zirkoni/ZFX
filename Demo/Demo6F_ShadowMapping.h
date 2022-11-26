#pragma once
#include "Demo6C_LightMaps.h"
#include <vector>


// Shadow mapping works best for directional light!
// Example is taken from: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping

class Demo6F : public Demo
{
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

public:
    Demo6F(ZFX::Camera& camera, ZFX::Window& window) : Demo{ camera, "Demo6F" }, m_window{window}
    {
        loadShaders();
        loadFloor();
        loadCubes();
        loadDepthBuffer();
        m_lightPos = glm::vec3{ -2.0f, 4.0f, -1.0f };
    }

    void draw() override
    {
        // 1. Render depth of scene to texture (from light's perspective)
        glm::mat4 lightProjection;
        glm::mat4 lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f;
        float far_plane = 7.5f;
        // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        m_simpleDepthShader->bind();
        m_simpleDepthShader->setUniformMat4("u_lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        m_depthBuffer.bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderDepth();
        m_depthBuffer.bindDefault();

        // reset viewport
        glViewport(0, 0, m_window.width(), m_window.height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. Render scene as normal using the generated depth/shadow map
        glm::mat4 viewProjection = m_camera.getViewProjection();
        m_shader->bind();
        m_shader->setUniformMat4("u_viewProjection", viewProjection);
        m_shader->setUniformVec3("u_viewPos", m_camera.position());
        m_shader->setUniformVec3("u_lightPos", m_lightPos);
        m_shader->setUniformMat4("u_lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        m_depthBuffer.bindDepthBuffer();
        renderScene();
    }

    void onEntry() override
    {
        m_camera.resetZoom();
        m_camera.position() = glm::vec3{-2.0f, 2.5f, 6.5f};
        m_camera.yaw() = -65.0f;
        m_camera.pitch() = -15.0f;
        m_camera.update();
    }

private:
    void loadShaders()
    {
        ZFX::ShaderSource shaderSrc = {SHADERS_PATH + "colour3D_ShadowMapping.vs", SHADERS_PATH + "colour3D_ShadowMapping.fs"};
        m_shader = std::make_shared<ZFX::Shader>(shaderSrc);

        ZFX::ShaderSource depthSrc = {SHADERS_PATH + "depthshader.vs", SHADERS_PATH + "depthshader.fs"};
        m_simpleDepthShader = std::make_unique<ZFX::Shader>(depthSrc);

        m_shader->bind();
        m_shader->setUniformInt("u_diffuseTexture", 0);
        m_shader->setUniformInt("u_shadowMap", 1);
    }

    void loadFloor()
    {
        ZFX::Verteces planeVertices =
        {
            ZFX::VertexData
            {
                // positions            // normals         // texcoords
                 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
                -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
                -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

                 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
                -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
                 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
            },

            ZFX::AttributeSizes{3, 3, 2}
        };

        ZFX::Indeces indeces = { 2, 1, 0,  5, 4, 3 };

        m_floor.load(planeVertices, indeces, m_shader);
        m_floor.loadTexture(TEXTURES_PATH + "texture.png");
    }

    void loadCubes()
    {
        m_cubes.load(cubeVertecesWithNormalsAndTexture(), cubeIndeces(), m_shader);
        m_cubes.loadTexture(TEXTURES_PATH + "container.png");

        // 1st cube
        auto& cube1 = m_cubes.transform();
        cube1.scale() = glm::vec3{0.5f};
        cube1.position() = glm::vec3{0.0f, 1.5f, 0.0f};

        // 2nd cube
        auto& cube2 = m_cubes.duplicate();
        cube2.position() = glm::vec3{2.0f, 0.0f, 1.0f};

        // 3rd cube
        auto& cube3 = m_cubes.duplicate();
        cube3.scale() = glm::vec3{0.25f};
        cube3.position() = glm::vec3{-1.0f, 0.0f, 2.0f};
        cube3.rotation().x = glm::radians(60.0f);
        cube3.rotation().z = glm::radians(60.0f);
    }

    void loadDepthBuffer()
    {
        m_depthBuffer.attachDepthBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);
    }

    void renderDepth()
    {
        auto& shader = *m_simpleDepthShader.get();
        m_floor.draw(shader);
        m_cubes.draw(shader);
    }

    void renderScene()
    {
        m_floor.draw(m_camera);
        m_cubes.draw(m_camera);
    }

private:
    ZFX::Window& m_window;
    std::shared_ptr<ZFX::Shader> m_shader;
    std::unique_ptr<ZFX::Shader> m_simpleDepthShader;
    ZFX::Object m_floor;
    ZFX::Object m_cubes;
    ZFX::FrameBuffer m_depthBuffer;
    glm::vec3 m_lightPos;
};
