#pragma once
#include <GL/glew.h>
#include <vector>
#include <utility>
#include <string>


namespace ZFX
{
    struct VertexAttribute
    {
        std::string name;
        GLint numElements;
    };

    using VertexData = std::vector<float>;
    using VertexAttributes = std::vector<VertexAttribute>;

    class Verteces
    {
    public:
        Verteces(VertexData& data, VertexAttributes& attributes);
        Verteces(VertexData&& data, VertexAttributes&& attributes);

        const VertexData& data() const { return m_data; }
        const VertexAttributes& attributes() const { return m_attributes; }
        uint32_t numElementsPerVertex() const;

    private:
        VertexData m_data;
        VertexAttributes m_attributes;
    };
}

