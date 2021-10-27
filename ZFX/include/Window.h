#pragma once
#include <string>
#include <SDL2/SDL.h>

namespace ZFX
{
    class Window
    {
    public:
        Window(const uint32_t width, const uint32_t height, const std::string& title);
        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
        ~Window();

        void clear(); // clear to black
        void clear(float r, float g, float b, float a); // clear to user defined colour
        void update();

        static float aspectRatio() { return ((float)s_width / s_height); }
        static uint32_t width() { return s_width; }
        static uint32_t height() { return s_height; }

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;

        static uint32_t s_width;
        static uint32_t s_height;
    };
}


