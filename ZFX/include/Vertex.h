#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>


namespace ZFX
{
	struct Vertex
	{
		/* Basic 2D constructor (z = 0.0) */
		Vertex(const glm::vec2& pos, const glm::vec4& col);

		/* 2D constructor with texture coordinates */
		Vertex(const glm::vec2& pos, const glm::vec4& col, const glm::vec2& tex);

		/* Basic 3D constructor */
		Vertex(const glm::vec3& pos, const glm::vec4& col);

		/* 3D constructor with texture coordinates */
		Vertex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& tex);

		virtual ~Vertex();

		/* OpenGL coordinates */
		glm::vec3 position;

		glm::vec4 colour;

		glm::vec2 texCoord;

		bool hasTexture;
	};
}

