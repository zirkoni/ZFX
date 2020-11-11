#pragma once
#include "Vertex.h"
#include <GL/glew.h>
#include <memory>


namespace ZFX
{
	class MeshBase
	{
		using VboList = std::unique_ptr< GLuint[] >;
	public:
		virtual ~MeshBase();

		void draw();

	protected:
		MeshBase(const uint32_t numBuffers, const uint32_t numIndeces);
		MeshBase(const MeshBase& other) = delete;
		MeshBase& operator=(const MeshBase& other) = delete;

	protected:
		uint32_t m_numBuffers;
		uint32_t m_numIndeces;
		GLuint m_vertexArrayObject;
		VboList m_vertexArrayBuffers;
	};


	class Mesh: public MeshBase
	{
	public:
		Mesh(Vertex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces);
		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;
	};


	class MeshTex : public MeshBase
	{
	public:
		MeshTex(VertexTex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces);
		MeshTex(const MeshTex& other) = delete;
		MeshTex& operator=(const MeshTex& other) = delete;
	};
}

