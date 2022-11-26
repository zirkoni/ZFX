#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace ZFX
{
    class Window
    {
    public:

        struct Options
        {
            uint32_t width = 800;
            uint32_t height = 600;
            std::string title = "";

            bool enableVsync = true;
            bool enableDepthTest = true;
            bool enableStencilTest = false;
            bool enableFaceCulling = true;
            bool enableBlending = true;
            bool enableMSAA = false;
            bool enableGlDebug = false;

            int rgbaSize = 8;
            int depthSize = 16;
            int stencilSize = 16;
            int numMSAASamples = 4;
        };

        Window(const Options& options);
        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
        ~Window();

        static void clear(); // clear to black
        static void clear(float r, float g, float b, float a); // clear to user defined colour
        static void clear(glm::vec4 bgColour); // clear to user defined colour

        void update();
        void setVsync(bool enabled);
        void setTitle(const std::string& title);

        static float aspectRatio() { return ((float)s_width / s_height); }
        static uint32_t width() { return s_width; }
        static uint32_t height() { return s_height; }

        void toggleFullscreen();

    private:
        void setGlAttributes(const Options& options);
        void setGlOptions(const Options& options);

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;

        static uint32_t s_width;
        static uint32_t s_height;
        static uint32_t s_bitsToClear;
    };
}


