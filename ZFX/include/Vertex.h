#pragma once
#include <glm/glm.hpp>


namespace ZFX
{
	/* Basic vertex */
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


	/* Vertex with texture coordinate */
	struct VertexTex : public Vertex
	{
		VertexTex(const glm::vec2& pos, const glm::vec4& col, const glm::vec2& tex);
		VertexTex(const glm::vec3& pos, const glm::vec4& col, const glm::vec2& tex);

		glm::vec2 texCoord;
	};
}

