#pragma once
#include "Demo.h"
#include "Window.h"

static const std::string FONT = FONTS_PATH + "arial.ttf";
static const std::string SHADER = SHADERS_PATH + "freetype";

class Demo3 : public Demo
{
public:
    Demo3(ZFX::Camera& camera) : Demo{ camera },
        m_textRenderer{ FONT, SHADER }
    {
    }

    void draw() override
    {
        m_textRenderer.drawText(
            "Hello World!",                          // text
            ZFX::Window::width() / 2.0f - 150.0f,    // x-coordinate (pixel coordinates)
            ZFX::Window::height() / 2.0f,            // y-coordinate (pixel coordinates)
            1.0f,                                    // scale
            glm::vec3(0.5f, 0.8f, 0.2f));            // text colour

        m_textRenderer.drawText("OpenGL Freetype text renderer", 5.0f, 10.0f, 0.5f, glm::vec3{ 0.0f, 0.8f, 0.8f });
    }

private:
    ZFX::TextFreetype m_textRenderer;
};
