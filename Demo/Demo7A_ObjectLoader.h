#pragma once
#include "Demo.h"

class Demo7A : public Demo
{
public:
    Demo7A(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo7A - Object Loader" }
    {
        m_speed = 0.5f;
        loadModel();

        for (auto& m : m_model.meshes())
        {
            m.transform().scale() = glm::vec3{ 0.5f };
        }
    }

    void update(float deltaTime) override
    {
        Demo::update(deltaTime);

        for (auto& m : m_model.meshes())
        {
            m.transform().rotation().z = m_delta;
            m.transform().rotation().x = m_delta;
        }
    }

    void draw() override
    {
        m_model.draw(m_camera);
    }

private:
    void loadModel()
    {
        const bool showWarnings = false;
        ZFX::ObjectLoader obj{ OBJS_PATH + "teamug/teamugobj.obj", showWarnings };
        m_model.load(obj.vertices(), obj.indices(), SHADERS_PATH + "colour3D_Lighting");
    }

private:
    ZFX::Object m_model;
};
