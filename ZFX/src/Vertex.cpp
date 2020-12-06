#include "Vertex.h"


ZFX::Vertex::Vertex(const glm::vec2& pos, const glm::vec4& col) :
    position{ pos.x, pos.y, 0.0f }, colour{ col }, texCoord{ 0.0f, 0.0f }, hasTexture{ false }
{
}

ZFX::Vertex::Vertex(const glm::vec2& pos, const glm::vec4& col, const glm::vec2& tex) :
    Vertex{ pos, col }
{
    hasTexture = true;
    texCoord = tex;
}

ZFX::Vertex::Vertex(const glm::vec3& pos, const glm::vec4& col) :
    position{ pos }, colour{ col }, texCoord{ 0.0f, 0.0f }, hasTexture{ false }
{
}

ZFX::Vertex::Vertex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& tex) :
    Vertex{ pos, col }
{
    hasTexture = true;
    texCoord = tex;
}

ZFX::Vertex::~Vertex()
{
}
