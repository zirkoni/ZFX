#pragma once
#include <string>
#include <SDL2\include\SDL.h>

namespace ZFX
{
	class Window
	{
	public:
		Window(const uint32_t width, const uint32_t height, const std::string& title);
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;
		~Window();

		void clear(float r, float g, float b, float a);
		void update();
		const float aspectRatio() const { return (float)(m_width / m_height); }

	private:
		SDL_Window* m_window;
		SDL_GLContext m_glContext;

		uint32_t m_width;
		uint32_t m_height;
	};
}


