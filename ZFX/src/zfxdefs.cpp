#include "zfxdefs.h"

namespace ZFX
{
void wireframeMode(/* const GLfloat lineWidth = 1.0f */)
{
    // glLineWidth(lineWidth); // Only width 1 is guaranteed to be supported
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void filledMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
}
