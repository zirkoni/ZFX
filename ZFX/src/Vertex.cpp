#include "Vertex.h"


ZFX::Vertex::Vertex(const glm::vec2& pos, const glm::vec4& col):
	position{ pos.x, pos.y, 0.0f }, colour{ col }
{
}

ZFX::Vertex::Vertex(const glm::vec3& pos, const glm::vec4& col):
	position{ pos }, colour{ col }
{
}

ZFX::Vertex::~Vertex()
{

}