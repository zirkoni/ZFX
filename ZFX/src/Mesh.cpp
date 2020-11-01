#include "Mesh.h"
#include "zfxdefs.h"

ZFX::Mesh::Mesh(Vertex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces):
	m_numIndeces{ numIndeces }
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

	// indeces
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndeces * sizeof(indeces[0]), indeces, GL_STATIC_DRAW);

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

	//glDrawArrays(GL_LINE_LOOP, 0, m_numIndeces); // wireframe
	//glDrawArrays(GL_TRIANGLES, 0, m_numIndeces); // fill with color
	glDrawElements(GL_TRIANGLES, m_numIndeces, GL_UNSIGNED_INT, 0); // with indeces

	glBindVertexArray(0);
}
