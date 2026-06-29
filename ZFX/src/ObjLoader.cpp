#if !defined(ZFX_NO_OBJECT_LOADER)

#include "ObjLoader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "Vertex.h"
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "zfxdefs.h"
#include "tiny_obj_loader.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

ZFX::ObjectLoader::ObjectLoader(const std::string& filename, bool smoothNormals, bool printWarnings): m_smoothNormals{smoothNormals}
{
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename))
    {
        if (!reader.Error().empty())
        {
            throw ZFX::Exception{ __FILE__, __LINE__, "TinyObjReader: " + reader.Error() };
        }

        throw ZFX::Exception{ __FILE__, __LINE__, "TinyObjReader: ParseFromFile failed" };
    }

    if (!reader.Warning().empty() && printWarnings)
    {
        std::cout << "TinyObjReader: " << reader.Warning() << "\n";
    }

    loadObject(reader);
}

void ZFX::ObjectLoader::loadObject(tinyobj::ObjReader& reader)
{
    // How many floats each property has
    constexpr unsigned numPositionValues = 3;
    constexpr unsigned numNormalValues = 3;
    constexpr unsigned numTextureValues = 2;
    constexpr unsigned totalNumValues = numPositionValues + numNormalValues + numTextureValues;

    VertexData data;
    AttributeSizes attributes;
    std::unordered_map<glm::vec3, uint32_t> vertexMap;
    bool hasNormals = false;
    bool hasUvs = false;

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    const auto& vertices = attrib.vertices;
    const auto& normals = attrib.normals;
    const auto& texcoords = attrib.texcoords;

    data.reserve(vertices.size() * totalNumValues);
    m_indeces.reserve(shapes[0].mesh.indices.size());

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            const float x = vertices[numPositionValues * index.vertex_index + 0];
            const float y = vertices[numPositionValues * index.vertex_index + 1];
            const float z = vertices[numPositionValues * index.vertex_index + 2];

            glm::vec3 coordinate{ x, y, z };

            bool newVertex = true;
            uint32_t vertexIndex = 0;

            if (m_smoothNormals)
            {
                auto it = vertexMap.find(coordinate);
                if (it != vertexMap.end())
                {
                    vertexIndex = it->second;
                    m_indeces.push_back(vertexIndex);
                    newVertex = false;
                }
            }

            if(newVertex)
            {
                vertexIndex = (uint32_t)vertexMap.size();
                vertexMap[coordinate] = vertexIndex;

                data.push_back(x);
                data.push_back(y);
                data.push_back(z);

                if (index.normal_index >= 0 && !normals.empty())
                {
                    const float nx = normals[numNormalValues * size_t(index.normal_index) + 0];
                    const float ny = normals[numNormalValues * size_t(index.normal_index) + 1];
                    const float nz = normals[numNormalValues * size_t(index.normal_index) + 2];
                    data.push_back(nx);
                    data.push_back(ny);
                    data.push_back(nz);
                    hasNormals = true;
                }

                if (index.texcoord_index >= 0 && !texcoords.empty())
                {
                    const float tx = texcoords[numTextureValues * size_t(index.texcoord_index) + 0];
                    const float ty = texcoords[numTextureValues * size_t(index.texcoord_index) + 1];
                    data.push_back(tx);
                    data.push_back(ty);
                    hasUvs = true;
                }

                m_indeces.push_back(vertexIndex);
            }
        }
    }

    attributes.push_back(numPositionValues);

    if (hasNormals)
    {
        attributes.push_back(numNormalValues);
    }

    if (hasUvs)
    {
        attributes.push_back(numTextureValues);
    }

    m_verteces.load(data, attributes);
}
#endif
