#pragma once
#include <glm/glm/glm.hpp>


namespace ZFX
{
	struct Vertex
	{
		/* 2D constructor (z = 0.0) */
		Vertex(const glm::vec2& pos);

		/* 3D constructor */
		Vertex(const glm::vec3& pos);

		virtual ~Vertex();

		/* GL coordinates */
		glm::vec3 position;
	};
}

