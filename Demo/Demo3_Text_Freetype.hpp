#pragma once
#include "Demo.hpp"
#include "Window.h"

static const std::string FONT =  "../../../Demo/Fonts/arial.ttf"; // "C:\\Windows\\Fonts\\arial.ttf";
static const std::string SHADER = "../../../Shaders/freetype";

class Demo3 : public Demo
{
public:
	Demo3(ZFX::Camera& camera) : Demo{ camera },
		m_textRenderer{ FONT, SHADER }
	{
	}

	void draw() override
	{
		// This function uses pixel coordinates!
		m_textRenderer.drawText(
			"Hello World!",                          // text
			ZFX::Window::width() / 2.0f - 150.0f,    // x-coordinate
			ZFX::Window::height() / 2.0f,            // y-coordinate
			1.0f,                                    // scale
			glm::vec3(0.5f, 0.8f, 0.2f));            // text colour

		m_textRenderer.drawText("OpenGL Freetype text renderer", 5.0f, 10.0f, 0.5f, glm::vec3{0.0f, 0.8f, 0.8f});
	}

private:
	ZFX::TextFreetype m_textRenderer;
};
