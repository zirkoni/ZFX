#pragma once
#include <iostream>


namespace ZFX
{
    class Camera;
}

std::ostream& operator<<(std::ostream& out, const ZFX::Camera& camera);

