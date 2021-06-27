#pragma once
#include "Mesh.h"
#include <string>

namespace tinyobj
{
    class ObjReader;
}

namespace ZFX
{
    class Object
    {
    public:
        Object(const std::string& filename);
        ~Object();

        const Verteces* verteces() const { return m_verteces; }
        const Indeces& indeces() const { return m_indeces; }

    private:
        void loadObject(tinyobj::ObjReader& reader);

    private:
        VertexData m_data;
        VertexAttributes m_attributes;
        Verteces* m_verteces;
        Indeces  m_indeces;
    };
}
