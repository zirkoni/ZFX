#pragma once
#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include "zfxPrint.h"
//#define USE_SDL2_IMAGE


namespace ZFX
{
    static constexpr GLint UNIFORM_NOT_FOUND = -1;

    /* Any subsequent drawing calls will render the triangles in wireframe mode */
    void wireframeMode(/* const GLfloat lineWidth = 1.0f */);

    /* Any subsequent drawing calls will render the triangles filled with colour */
    void filledMode();

    class Exception: public std::runtime_error
    {
    public:
        Exception(const std::string& msg): std::runtime_error{msg} {}
        Exception(const char* file, int line, const std::string& msg):
            std::runtime_error{std::string(file) + ":" + std::to_string(line) + ": " + msg} {}
    };
}
