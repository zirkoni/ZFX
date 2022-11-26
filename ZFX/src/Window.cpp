#include "Window.h"
#include "Camera.h"
#include "zfxdefs.h"
#include "glDebug.h"
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <iostream>


uint32_t ZFX::Window::s_width = 0;
uint32_t ZFX::Window::s_height = 0;
uint32_t ZFX::Window::s_bitsToClear = GL_COLOR_BUFFER_BIT;

ZFX::Window::Window(const Options& options) :
    m_window{ nullptr },
    m_glContext{ nullptr }
{
    s_width = options.width;
    s_height = options.height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::string msg = "SDL_Init failed: ";
        msg += std::string{ SDL_GetError() };
        throw ZFX::Exception{ __FILE__, __LINE__, msg };
    }

#ifdef USE_SDL2_IMAGE
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        std::string msg = "IMG_Init failed: ";
        msg += std::string{ IMG_GetError() };
        throw ZFX::Exception{ __FILE__, __LINE__, msg };
    }
#endif

    setGlAttributes(options);

    m_window = SDL_CreateWindow(options.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        s_width, s_height, SDL_WINDOW_OPENGL);

    if (nullptr == m_window)
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "SDL_CreateWindow failed" };
    }

    m_glContext = SDL_GL_CreateContext(m_window);

    if (nullptr == m_glContext)
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "SDL_CreateWindow failed" };
    }

    if (GLEW_OK != glewInit())
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "glewInit() failed" };
    }

    setGlOptions(options);
    setVsync(options.enableVsync);

    if(options.enableGlDebug)
    {
        enableGlDebug();
    }
}

ZFX::Window::~Window()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void ZFX::Window::setGlAttributes(const Options& options)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, options.rgbaSize);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, options.rgbaSize);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, options.rgbaSize);

    if(options.enableBlending)
    {
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, options.rgbaSize);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 4 * options.rgbaSize);
    } else
    {
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 3 * options.rgbaSize);
    }

    if(options.enableDepthTest)
    {
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, options.depthSize);
    }

    if(options.enableStencilTest)
    {
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, options.stencilSize);
    }

    if(options.enableMSAA)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, options.numMSAASamples);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void ZFX::Window::setGlOptions(const Options& options)
{
    if(options.enableDepthTest && options.enableFaceCulling)
    {
        // for simple object these are enough
        // Cull faces that are facing away from the camera
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    if(options.enableDepthTest)
    {
        // Complex object require depth test:
        // don't draw pixels that are behind pixels that are closer to the camera.
        glEnable(GL_DEPTH_TEST);
    }

    if(options.enableStencilTest)
    {
        glEnable(GL_STENCIL_TEST);
        // Set some default options:
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    if(options.enableBlending)
    {
        // Enable transparency (alpha channel)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if(options.enableMSAA)
    {
        glEnable(GL_MULTISAMPLE);
    }

    s_bitsToClear = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT * options.enableDepthTest | GL_STENCIL_BUFFER_BIT * options.enableStencilTest;
}

void ZFX::Window::setVsync(bool enabled)
{
    SDL_GL_SetSwapInterval((enabled ? 1 : 0));
}

void ZFX::Window::clear()
{
    clear(0.0f, 0.0f, 0.0f, 1.0f);
}

void ZFX::Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(s_bitsToClear);
}

void ZFX::Window::clear(glm::vec4 bgColour)
{
    clear(bgColour.r, bgColour.g, bgColour.b, bgColour.a);
}

void ZFX::Window::update()
{
    SDL_GL_SwapWindow(m_window);
}

void ZFX::Window::toggleFullscreen()
{
    const Uint32 flags = SDL_GetWindowFlags(m_window);
    const bool isFullScreen = flags & SDL_WINDOW_FULLSCREEN;
    const bool isBorderlessFullScreen = flags & SDL_WINDOW_FULLSCREEN_DESKTOP;

    if(isFullScreen or isBorderlessFullScreen)
    {
        Camera::resize(aspectRatio());
        SDL_SetWindowFullscreen(m_window, 0);
    } else
    {
        SDL_DisplayMode dispMode;
        int displayIdx = SDL_GetWindowDisplayIndex(m_window);
        SDL_GetCurrentDisplayMode(displayIdx, &dispMode);

        Camera::resize((float)dispMode.w / dispMode.h);
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    }
}

void ZFX::Window::setTitle(const std::string &title)
{
    SDL_SetWindowTitle(m_window, title.c_str());
}
