#include "Window.h"
#include "zfxdefs.h"
#include <GL/glew.h>
#include <SDL_image.h>
#include <iostream>
#include <stdexcept>

uint32_t ZFX::Window::s_width = 0;
uint32_t ZFX::Window::s_height = 0;

ZFX::Window::Window(const uint32_t width, const uint32_t height, const std::string& title) :
    m_window{ nullptr },
    m_glContext{ nullptr }
{
    s_width = width;
    s_height = height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::string msg = "SDL_Init failed: ";
        msg += std::string{ SDL_GetError() };
        throw std::runtime_error{ msg };
    }

#ifdef USE_SDL2_IMAGE
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        std::string msg = "IMG_Init failed: ";
        msg += std::string{ IMG_GetError() };
        throw std::runtime_error{ msg };
    }
#endif

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // 4 * 8 = 32 (RGBA)

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        s_width, s_height, SDL_WINDOW_OPENGL);

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
    else
    {
        // for simple object these are enough
        // Cull faces that are facing away from the camera
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Enable transparency (alpha channel)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
