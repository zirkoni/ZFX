#pragma once
#include <glm/glm/glm.hpp>


namespace ZFX
{
	struct Vertex
	{
		/* 2D constructor (z = 0.0) */
		Vertex(const glm::vec2& pos, const glm::vec4& col);

		/* 3D constructor */
		Vertex(const glm::vec3& pos, const glm::vec4& col);

		virtual ~Vertex();

		/* OpenGL coordinates */
		glm::vec3 position;

		glm::vec4 colour;
	};
}

