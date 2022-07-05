#pragma once
#include "Mesh.h"
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
        ObjectLoader(const std::string& filename, bool smoothNormals = true);
        ~ObjectLoader();

        const Verteces* verteces() const { return m_verteces; }
        const Indeces& indeces() const { return m_indeces; }

    private:
        void loadObject(tinyobj::ObjReader& reader);

    private:
        bool m_smoothNormals;
        VertexData m_data;
        AttributeSizes m_attributes;
        Verteces* m_verteces;
        Indeces  m_indeces;
    };
}
