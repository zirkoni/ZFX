#include "Mesh.h"
#include "zfxdefs.h"


ZFX::MeshBase::MeshBase(const uint32_t numBuffers, const uint32_t numIndeces):
	m_numBuffers{ numBuffers }, m_numIndeces{ numIndeces },
	m_vertexArrayObject{ 0 }, m_vertexArrayBuffers{new GLuint[numBuffers] }
{
}

ZFX::MeshBase::~MeshBase()
{
	glDeleteBuffers(m_numBuffers, m_vertexArrayBuffers.get());
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void ZFX::MeshBase::draw()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_numIndeces, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


ZFX::Mesh::Mesh(Vertex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces) :
	MeshBase{ 2, numIndeces }
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// move data to GPU
	glGenBuffers(m_numBuffers, m_vertexArrayBuffers.get());
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.get()[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// tell GPU how to interpret the data
	glEnableVertexAttribArray(POSITION_VA);
	glVertexAttribPointer(POSITION_VA, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

	glEnableVertexAttribArray(COLOUR_VA);
	glVertexAttribPointer(COLOUR_VA, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, colour));

	// indeces
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers.get()[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndeces * sizeof(indeces[0]), indeces, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


ZFX::MeshTex::MeshTex(VertexTex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces) :
	MeshBase{ 2, numIndeces }
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// move data to GPU
	glGenBuffers(m_numBuffers, m_vertexArrayBuffers.get());
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.get()[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// tell GPU how to interpret the data
	glEnableVertexAttribArray(POSITION_VA);
	glVertexAttribPointer(POSITION_VA, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTex), (GLvoid*)offsetof(VertexTex, position));

	glEnableVertexAttribArray(COLOUR_VA);
	glVertexAttribPointer(COLOUR_VA, 4, GL_FLOAT, GL_FALSE, sizeof(VertexTex), (GLvoid*)offsetof(VertexTex, colour));

	glEnableVertexAttribArray(TEXTURE_VA);
	glVertexAttribPointer(TEXTURE_VA, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTex), (GLvoid*)offsetof(VertexTex, texCoord));

	// indeces
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers.get()[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndeces * sizeof(indeces[0]), indeces, GL_STATIC_DRAW);

	glBindVertexArray(0);
}