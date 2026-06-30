#pragma once
#include "Demo.h"

class Demo7A : public Demo
{
public:
    Demo7A(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo7A - Object Loader" }
    {
        loadModel();

        for (auto& m : m_model.meshes())
        {
            m.transform().scale() = glm::vec3{ 0.5f };
        }
    }

    void update() override
    {
        for (auto& m : m_model.meshes())
        {
            m.transform().rotation().z = m_counter;
            m.transform().rotation().x = m_counter;
        }

        m_counter += 0.001f;
    }

    void draw() override
    {
        m_model.draw(m_camera);
    }

private:
    void loadModel()
    {
        ZFX::ObjectLoader obj{ OBJS_PATH + "teamug/teamugobj.obj", true, false };
        m_model.load({obj.vertices()}, {obj.indices()}, SHADERS_PATH + "colour3D_Lighting");
    }

private:
    ZFX::Object m_model;
};
