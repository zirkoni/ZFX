#pragma once
#include "Demo3_Square.h"


class Demo9 : public Demo3
{
public:
    Demo9(ZFX::Window& window, ZFX::Camera& camera) : Demo3{ window, camera, "Demo9 - Mouse" }
    {
        // TODO: Demo for converting between SDL (mouse position) and OpenGL coordinates?
    }
};
