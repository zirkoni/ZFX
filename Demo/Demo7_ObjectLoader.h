#pragma once
#include "Demo.h"
#include <vector>

class Demo7 : public Demo
{
public:
    Demo7(ZFX::Camera& camera) : Demo{ camera, "Demo7" }
    {
        addCube();
        m_cube.transform().scale() = glm::vec3{ 0.5f };
    }

    void draw() override
    {
        m_cube.transform().rotation().z = m_counter;
        m_cube.transform().rotation().x = m_counter;
        m_counter += 0.001f;

        m_cube.draw(m_camera);
    }

private:
    void addCube()
    {
        //ZFX::ObjectLoader obj{ OBJS_PATH + "cube.obj", false };
        //ZFX::ObjectLoader obj{ OBJS_PATH + "monkey/monkey3_normals.obj" };
        ZFX::ObjectLoader obj{ OBJS_PATH + "teamug/teamugobj.obj" };
        m_cube.load(*obj.verteces(), obj.indeces(), SHADERS_PATH + "colour3D_Lighting");
    }

private:
    ZFX::Object m_cube;
};
