#include "Vertex.h"


ZFX::Vertices::Vertices(VertexData& data, AttributeSizes& attributes): m_data{data}, m_attributes{attributes}
{
}

ZFX::Vertices::Vertices(VertexData&& data, AttributeSizes&& attributes)
{
    m_data = std::move(data);
    m_attributes = std::move(attributes);
}

void ZFX::Vertices::load(VertexData& data, AttributeSizes& attributes)
{
    m_data = std::move(data);
    m_attributes = std::move(attributes);
}

uint32_t ZFX::Vertices::numElementsPerVertex() const
{
    uint32_t sum = 0;
    for (const auto& atr : m_attributes)
    {
        sum += atr;
    }

    return sum;
}
