#pragma once
#include "Demo.h"

// Use the same Survival Guitar Backpack by Berk Gedik as
// https://learnopengl.com/Model-Loading/Model

class Demo7B : public Demo
{
public:
    Demo7B(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo7B - Backback" }
    {
        loadBackpackModel();
        addLighting();
        m_backpack.transform().scale() = glm::vec3{ 0.5f };
    }

    void update() override
    {
        m_backpack.transform().rotation().y = m_counter;
        m_counter += 0.005f;

        updateLighting();
    }

    void draw() override
    {
        m_backpack.draw(m_camera);
    }

private:
    void loadBackpackModel()
    {
        ZFX::ObjectLoader obj{ OBJS_PATH + "backpack/backpack.obj" };
        m_backpack.load(obj.verteces(), obj.indeces(), SHADERS_PATH + "colour3D_NormalMapping");
        m_backpack.shader().bind();
        m_backpack.shader().setUniform("u_material.shininess", 32.0f);

        m_backpack.loadTexture(OBJS_PATH + "backpack/diffuse.jpg");
        m_backpack.loadTexture(OBJS_PATH + "backpack/specular.jpg");
        m_backpack.loadTexture(OBJS_PATH + "backpack/normal.png");
    }

    void addLighting()
    {
        m_backpack.shader().bind();
        m_backpack.shader().setUniform("u_viewPosition", m_camera.position());

        // Not using the PointLight class.
        // Maybe the light classes could be removed, clearer like this?

        // 1st light
        m_backpack.shader().setUniform("u_pointLights[0].position", glm::vec3{ 2.0f, 2.0f, 2.0f });
        m_backpack.shader().setUniform("u_pointLights[0].ambient", glm::vec3{ 0.2f, 0.2f, 0.2f });
        m_backpack.shader().setUniform("u_pointLights[0].diffuse", glm::vec3{ 0.8f, 0.7f, 0.5f });
        m_backpack.shader().setUniform("u_pointLights[0].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
        m_backpack.shader().setUniform("u_pointLights[0].constant", 1.0f);
        m_backpack.shader().setUniform("u_pointLights[0].linear", 0.09f);
        m_backpack.shader().setUniform("u_pointLights[0].quadratic", 0.032f);

        // 2nd light
        m_backpack.shader().setUniform("u_pointLights[1].position", glm::vec3{ -1.5f, 1.5f, 2.0f });
        m_backpack.shader().setUniform("u_pointLights[1].ambient", glm::vec3{ 0.1f, 0.1f, 0.15f });
        m_backpack.shader().setUniform("u_pointLights[1].diffuse", glm::vec3{ 0.4f, 0.5f, 0.7f });
        m_backpack.shader().setUniform("u_pointLights[1].specular", glm::vec3{ 0.8f, 0.8f, 0.8f });
        m_backpack.shader().setUniform("u_pointLights[1].constant", 1.0f);
        m_backpack.shader().setUniform("u_pointLights[1].linear", 0.09f);
        m_backpack.shader().setUniform("u_pointLights[1].quadratic", 0.032f);
    }

    void updateLighting()
    {
        m_backpack.shader().bind();
        m_backpack.shader().setUniform("u_viewPosition", m_camera.position());
    }

private:
    ZFX::Object m_backpack;
};
