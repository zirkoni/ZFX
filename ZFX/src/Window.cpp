#include "Window.h"
#include "zfxdefs.h"
#include "glDebug.h"
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <iostream>


uint32_t ZFX::Window::s_width = 0;
uint32_t ZFX::Window::s_height = 0;

ZFX::Window::Window(const uint32_t width, const uint32_t height, const std::string& title, bool vsync, bool glDebug) :
    m_window{ nullptr },
    m_glContext{ nullptr }
{
    s_width = width;
    s_height = height;

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

    setGlAttributes();

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

    setGlewAttributes();
    setVsync(vsync);

    if(glDebug)
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

void ZFX::Window::setGlAttributes()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // 4 * 8 = 32 (RGBA)

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // If GL_DEPTH_TEST is enabled

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void ZFX::Window::setGlewAttributes()
{
    // for simple object these are enough
    // Cull faces that are facing away from the camera
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Complex object require depth test:
    // don't draw pixels that are behind pixels that are closer to the camera.
    glEnable(GL_DEPTH_TEST);

    // Enable transparency (alpha channel)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ZFX::Window::clear(glm::vec4 bgColour)
{
    clear(bgColour.r, bgColour.g, bgColour.b, bgColour.a);
}

void ZFX::Window::update()
{
    SDL_GL_SwapWindow(m_window);
}
