#pragma once
#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>
//#define USE_SDL2_IMAGE


namespace ZFX
{
    static constexpr GLint UNIFORM_NOT_FOUND = -1;
    static bool drawFilled = true;

    /* Any subsequent drawing calls will render the triangles in wireframe mode */
    static void wireframeMode(/* const GLfloat lineWidth = 1.0f */)
    {
        // glLineWidth(lineWidth); // Only width 1 is guaranteed to be supported
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawFilled = false;
    }

    /* Any subsequent drawing calls will render the triangles filled with colour */
    static void filledMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawFilled = true;
    }

    class Exception: public std::runtime_error
    {
    public:
        Exception(const std::string& msg): std::runtime_error{msg} {}
        Exception(const char* file, int line, const std::string& msg):
            std::runtime_error{std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + msg} {}
    };
}
