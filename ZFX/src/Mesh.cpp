#include "Mesh.h"
#include "zfxdefs.h"


ZFX::Mesh::Mesh(const Verteces& vertices, const Indeces& indeces):
	m_numIndeces{ (GLsizei)indeces.size() }, m_vertexArrayObject{ 0 }
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// move data to GPU
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.at(0)), vertices.data(), GL_STATIC_DRAW);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndeces * sizeof(indeces.at(0)), indeces.data(), GL_STATIC_DRAW);

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
