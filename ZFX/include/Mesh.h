#pragma once
#include "Vertex.h"
#include <GL/glew.h>
#include <memory>


namespace ZFX
{
	class Mesh
	{
	public:
		Mesh(Vertex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces);
		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;
		~Mesh();

		void draw();

	private:
		enum
		{
			VERTEX_BUFFER,
			INDEX_BUFFER,

			NUM_BUFFERS
		};

		uint32_t m_numIndeces;
		GLuint m_vertexArrayObject;
		GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	};
}

