#pragma once
#include "Demo.h"
#include <vector>

class Demo7 : public Demo
{
public:
    Demo7(ZFX::Camera& camera) : Demo{ camera, "Demo7" }
    {
        m_cube = addCube();
        m_cube->transform.scale() = glm::vec3{ 0.5f };
    }

    void draw() override
    {
        m_cube->transform.rotation().z = m_counter;
        m_cube->transform.rotation().x = m_counter;
        m_counter += 0.001f;

        m_cube->draw(m_camera);
    }

private:
    std::unique_ptr<BasicShape> addCube()
    {
        //ZFX::Object obj{ OBJS_PATH + "cube.obj", false };
        //ZFX::Object obj{ OBJS_PATH + "monkey/monkey3_normals.obj" };
        ZFX::Object obj{ OBJS_PATH + "teamug/teamugobj.obj" };

        return std::make_unique<BasicShape>(*obj.verteces(), obj.indeces(), "colour3D_Lighting");
    }

private:
    std::unique_ptr<BasicShape> m_cube;
};
