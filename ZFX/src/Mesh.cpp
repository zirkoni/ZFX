#include "Mesh.h"
#include "zfxdefs.h"


ZFX::Mesh::Mesh(Vertex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces):
	m_numIndeces{ numIndeces }, m_vertexArrayObject{ 0 }
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// move data to GPU
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// tell GPU how to interpret the data
	glEnableVertexAttribArray(POSITION_VA);
	glVertexAttribPointer(POSITION_VA, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

	glEnableVertexAttribArray(COLOUR_VA);
	glVertexAttribPointer(COLOUR_VA, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, colour));

	if (vertices[0].hasTexture)
	{
		glEnableVertexAttribArray(TEXTURE_VA);
		glVertexAttribPointer(TEXTURE_VA, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
	}

	// indeces
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndeces * sizeof(indeces[0]), indeces, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ZFX::Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void ZFX::Mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_numIndeces, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
