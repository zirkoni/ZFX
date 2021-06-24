#pragma once
#include <GL/glew.h>
//#define USE_SDL2_IMAGE

namespace ZFX
{
    static constexpr GLint UNIFORM_NOT_FOUND = -1;

    /* Any subsequent drawing calls will render the triangles in wireframe mode */
    static void wireframeMode(/* const GLfloat lineWidth = 1.0f */)
    {
        // glLineWidth(lineWidth); // Only width 1 is guaranteed to be supported
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    /* Any subsequent drawing calls will render the triangles filled with colour */
    static void filledMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
