#pragma once
#include "Demo6C_LightMaps.h"
#include <vector>


// Omnidirectional shadow map
// Example is taken from: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

class Demo6G : public Demo
{
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

public:
    Demo6G(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo6G - Multidirectional Shadow Mapping" }
    {
        loadShaders();
        loadCubes();
        loadDepthBuffer();
        m_lightPos = glm::vec3{ 0.0f };

        onEntry();
    }

    void draw() override
    {
        // Do here instead of in loadShaders.
        // This fixes occasional black screen, no idea why.
        static bool lateInitWorkaround = true;
        if(lateInitWorkaround)
        {
            m_shader->bind();
            m_shader->setUniform("u_diffuseTexture", 0);
            m_shader->setUniform("u_depthMap", 1);

            /* Validate after setting the above uniforms.
            *  This fixes error on Intel integrated graphics:
            *  glValidateProgram failed:
            *  active samplers with a different type refer to the same texture image unit
            *  TODO: should always validate only in debug build + only when requested by user (before start drawing)?
            */
            m_shader->validate();
            lateInitWorkaround = false;
        }

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
        m_simpleDepthShader->bind();
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_simpleDepthShader->setUniform("u_shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        }
        m_simpleDepthShader->setUniform("u_farPlane", farPlane);
        m_simpleDepthShader->setUniform("u_lightPos", m_lightPos);
        renderDepth();
        m_depthBuffer.bindDefault();

        // 2. render scene as normal
        glViewport(0, 0, m_window.width(), m_window.height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 viewProjection = m_camera.getViewProjection();
        m_shader->bind();
        m_shader->setUniform("u_viewProjection", viewProjection);
        m_shader->setUniform("u_viewPos", m_camera.position());
        m_shader->setUniform("u_lightPos", m_lightPos);
        m_shader->setUniform("u_farPlane", farPlane);
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
        m_shader = std::make_shared<ZFX::Shader>(shaderSrc, false); // do not validate shader in constructor

        // Vertex + fragment + geometry shader
        ZFX::ShaderSource depthSrc = {SHADERS_PATH + "depthCube.vs", SHADERS_PATH + "depthCube.fs", SHADERS_PATH + "depthCube.gs"};
        m_simpleDepthShader = std::make_unique<ZFX::Shader>(depthSrc);
    }

    void loadCubes()
    {
        m_room.load(cubeVertecesWithNormalsAndTexture(), cubeIndeces(), m_shader);
        m_room.loadTexture(TEXTURES_PATH + "texture.png");
        m_room.transform().scale() = glm::vec3{5.0f};

        m_cubes.load(cubeVertecesWithNormalsAndTexture(), cubeIndeces(), m_shader);
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
        m_shader->setUniform("u_reverseNormals", 1);
        m_room.draw(m_camera);
        m_shader->setUniform("u_reverseNormals", 0); // and of course disable it
        glEnable(GL_CULL_FACE);

        m_cubes.draw(m_camera);
    }

private:
    std::shared_ptr<ZFX::Shader> m_shader;
    std::unique_ptr<ZFX::Shader> m_simpleDepthShader;
    ZFX::Object m_room;
    ZFX::Object m_cubes;
    ZFX::FrameBuffer m_depthBuffer;
    glm::vec3 m_lightPos;
};
