#pragma once
#include <GL/glew.h>
#include <vector>
#include <utility>
#include <string>


namespace ZFX
{
    using VertexData = std::vector<float>;
    using AttributeSizes = std::vector<GLint>;

    class Verteces
    {
    public:
        Verteces(VertexData& data, AttributeSizes& attributes);
        Verteces(VertexData&& data, AttributeSizes&& attributes);

        const VertexData& data() const { return m_data; }
        const AttributeSizes& attributes() const { return m_attributes; }
        uint32_t numElementsPerVertex() const;

    private:
        VertexData m_data;
        AttributeSizes m_attributes;
    };
}

