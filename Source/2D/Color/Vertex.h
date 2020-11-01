#pragma once
#include <glm/glm/glm.hpp>


namespace ZFX_2D
{
	namespace Color
	{
		struct Vertex
		{
			Vertex(const glm::vec2& pos);
			virtual ~Vertex();

			glm::vec3 position;
		};
	}
}

