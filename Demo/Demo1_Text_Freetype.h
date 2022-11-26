#pragma once
#include "Demo.h"
#include "Window.h"


static const std::string FONT = FONTS_PATH + "arial.ttf";

class Demo1 : public Demo
{
public:
    Demo1(ZFX::Camera& camera) : Demo{ camera, "Demo1 - Rendering Text" },
        m_textRenderer{ FONT }
    {
        m_strings.push_back("<Space> : Next demo");
        m_strings.push_back("<Arrow keys> : Move camera");
        m_strings.push_back("<C> : Toggle background colour (black/white)");
        m_strings.push_back("<D> : Next scene (only frame buffers demo)");
        m_strings.push_back("<W> : Toggle wireframe mode");
        m_strings.push_back("<Z> : Reset zoom");
        m_strings.push_back("");
        m_strings.push_back("Mouse wheel : zoom in/out");
        m_strings.push_back("Mouse middle button + mouse movement : turn camera");
    }

    void draw() override
    {
        m_textRenderer.drawText(
            "Hello World!",                          // text
            ZFX::Window::width() / 2.0f - 150.0f,    // x-coordinate (pixel coordinates)
            ZFX::Window::height() - 50.0f,           // y-coordinate (pixel coordinates)
            1.0f,                                    // scale
            glm::vec4(0.5f, 0.8f, 0.2f, 0.5f));      // text colour

        float yCoord = ZFX::Window::height() - 100.0f;
        for(const auto& str : m_strings)
        {
            m_textRenderer.drawText(str, 20.0f, yCoord, 0.5f, ZFX::YELLOW);
            yCoord -= 30.0f;
        }
    }

private:
    ZFX::TextFreetype m_textRenderer;
    std::vector<std::string> m_strings;
};
