#include "Vertex.h"


ZFX::Verteces::Verteces(VertexData& data, VertexAttributes& attributes): m_data{data}, m_attributes{attributes}
{
}

ZFX::Verteces::Verteces(VertexData&& data, VertexAttributes&& attributes)
{
    m_data = std::move(data);
    m_attributes = std::move(attributes);
}

uint32_t ZFX::Verteces::numElementsPerVertex() const
{
    uint32_t sum = 0;
    for (const auto& atr : m_attributes)
    {
        sum += atr.numElements;
    }

    return sum;
}
