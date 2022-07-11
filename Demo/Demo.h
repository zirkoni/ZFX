#pragma once
#include "ZFX.h"
#include "Constants.h"


class Demo
{
public:
    Demo(ZFX::Camera& camera, const std::string& name): m_counter{ 0.0f },
    m_camera{ camera }, m_name{ name }, m_bgColour{ZFX::BLACK} {}

    virtual ~Demo() {}

    //virtual void update(float deltaTime) = 0; // TODO
    virtual void draw() = 0;

    virtual void onExit() {}
    virtual void onEntry()
    {
        m_camera.position().z = 3.0f;
        m_camera.resetZoom();
    }

    const std::string& name() { return m_name; }

    void changeBgColour()
    {
        if(m_bgColour == ZFX::BLACK)
        {
            m_bgColour = ZFX::WHITE;
        } else
        {
            m_bgColour = ZFX::BLACK;
        }
    }

    const glm::vec4& bgColour() const { return m_bgColour; }

protected:
    float m_counter;
    ZFX::Camera& m_camera;
    std::string m_name;

    glm::vec4 m_bgColour;
};
