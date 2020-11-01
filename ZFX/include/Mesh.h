#pragma once
#include "Vertex.h"
#include <glew/include/GL/glew.h>


namespace ZFX
{
	class Mesh
	{
	public:
		Mesh(Vertex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces);
		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;
		virtual ~Mesh();

		void draw();

	protected:
		enum
		{
			VERTEX_BUFFER,
			INDEX_BUFFER,

			NUM_BUFFERS
		};

		GLuint m_vertexArrayObject;
		GLuint m_vertexArrayBuffers[NUM_BUFFERS];
		unsigned int m_numIndeces;
	};
}

