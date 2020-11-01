#include "Window.h"
#include <glew\include\GL\glew.h>
#include <iostream>
#include <stdexcept>


ZFX::Window::Window(const uint32_t width, const uint32_t height, const std::string& title):
	m_width{ width },
	m_height{ height },
	m_window{ nullptr },
	m_glContext{ nullptr }
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // 4 * 8 = 32 (RGBA)

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_width, m_height, SDL_WINDOW_OPENGL);

	if (nullptr == m_window)
	{
		throw std::runtime_error{ "SDL_CreateWindow failed" };
	}
	
	m_glContext = SDL_GL_CreateContext(m_window);

	if (nullptr == m_glContext)
	{
		throw std::runtime_error{ "SDL_CreateWindow failed" };
	}

	if (GLEW_OK != glewInit())
	{
		throw std::runtime_error{ "glewInit() failed" };
	}
}

ZFX::Window::~Window()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}

void ZFX::Window::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ZFX::Window::update()
{
	SDL_GL_SwapWindow(m_window);
}
