#pragma once
#include "ZFX.h"
#include "Constants.h"


class Demo
{
public:
    Demo(ZFX::Window& window, ZFX::Camera& camera, const std::string& name): m_counter{ 0.0f },
    m_window{ window }, m_camera{ camera }, m_name{ name }, m_bgColour{ZFX::BLACK} {}

    virtual ~Demo() {}

    virtual void handleInput(const SDL_Event& e) {}
    virtual void draw() = 0;

    virtual void onExit() {}
    virtual void onEntry()
    {
        m_camera.position() = glm::vec3{0.0f, 0.0f, 3.0f};
        m_camera.resetZoom();
        m_camera.yaw() = -90.0f;
        m_camera.pitch() = 0.0f;
        m_camera.update();
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
    ZFX::Window& m_window;
    ZFX::Camera& m_camera;
    std::string m_name;

    glm::vec4 m_bgColour;
};
