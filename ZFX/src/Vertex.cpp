#include "Vertex.h"


ZFX::Vertex::Vertex(const glm::vec2& pos) : position{ pos.x, pos.y, 0.0f }
{
}

ZFX::Vertex::Vertex(const glm::vec3& pos): position{ pos }
{
}

ZFX::Vertex::~Vertex()
{

}