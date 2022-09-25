#include "zfxPrint.h"
#include "Camera.h"


std::ostream& operator<<(std::ostream& out, const ZFX::Camera& camera)
{
    const auto& pos = camera.position();

    out << "pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ") - yaw: "
        << camera.yaw() << ", pitch: " << camera.pitch();

    return out;
}

