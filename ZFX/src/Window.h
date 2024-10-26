#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace ZFX
{
    class Window
    {
    public:

        struct Options
        {
            // Window
            uint32_t width = 800;
            uint32_t height = 600;
            std::string title = "";

            bool fullscreen = false;
            bool fullscreenDesktop = false;
            bool borderless = false;
            bool resizable = false;
            bool minimized = false;
            bool maximized = false;
            bool inputGrabbed = false;

            // OpenGL
            bool enableVsync = true;
            bool enableDepthTest = true;
            bool enableStencilTest = false;
            bool enableFaceCulling = true;
            bool enableBlending = true;
            bool enableMSAA = false;
            bool enableGlDebug = false;

            int rgbaSize = 8;
            int depthSize = 24; // Same as in framebuffer
            int stencilSize = 8; // Same as in framebuffer
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
        SDL_DisplayMode getCurrentDisplayMode();
        const std::vector<SDL_DisplayMode> getSupportedDisplayModes();
        void resize(const SDL_DisplayMode& mode);
        void userResized();

        SDL_Window* sdlWindow() { return m_window; }

    private:
        void setGlAttributes(const Options& options);
        void setGlOptions(const Options& options);
        Uint32 windowFlags(const Options& options);

        SDL_Window* m_window;
        SDL_GLContext m_glContext;

        static uint32_t s_width;
        static uint32_t s_height;
        static uint32_t s_bitsToClear;
    };
}


