#pragma once
#include "Demo.h"
#include "Window.h"


static const std::string FONT = FONTS_PATH + "arial.ttf";

class Demo3 : public Demo
{
public:
    Demo3(ZFX::Camera& camera) : Demo{ camera, "Demo3" },
        m_textRenderer{ FONT }
    {
    }

    void draw() override
    {
        m_textRenderer.drawText(
            "Hello World!",                          // text
            ZFX::Window::width() / 2.0f - 150.0f,    // x-coordinate (pixel coordinates)
            ZFX::Window::height() / 2.0f,            // y-coordinate (pixel coordinates)
            1.0f,                                    // scale
            glm::vec4(0.5f, 0.8f, 0.2f, 0.5f));      // text colour

        m_textRenderer.drawText("OpenGL Freetype text renderer", 5.0f, 10.0f, 0.5f, ZFX::YELLOW);
    }

private:
    ZFX::TextFreetype m_textRenderer;
};
