#pragma once
#include "Mesh.h"
#include "Object.h"
#include <string>

namespace tinyobj
{
    class ObjReader;
}

namespace ZFX
{
    class ObjectLoader
    {
    public:
        ObjectLoader(const std::string& filename, bool smoothNormals = true, bool printWarnings = true);

        const auto& vertices() const { return m_vertices; }
        const auto& indices() const { return m_indices; }

    private:
        void loadObject(tinyobj::ObjReader& reader);

    private:
        bool m_smoothNormals;
        Object::VertexList m_vertices;
        Object::IndexList  m_indices;
    };
}
