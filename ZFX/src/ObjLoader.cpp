#include "ObjLoader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "../tinyobjloader/tiny_obj_loader.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

ZFX::Object::Object(const std::string& filename): m_verteces{nullptr}
{
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename))
    {
        if (!reader.Error().empty())
        {
            throw std::runtime_error{ std::string{ "TinyObjReader: " } + reader.Error() };
        }

        throw std::runtime_error{ "TinyObjReader: ParseFromFile failed" };
    }

    if (!reader.Warning().empty())
    {
        // do nothing...
    }

    loadObject(reader);
}

ZFX::Object::~Object()
{
    if (m_verteces)
    {
        delete m_verteces;
        m_verteces = nullptr;
    }
}

void ZFX::Object::loadObject(tinyobj::ObjReader& reader)
{
    VertexData data;
    VertexAttributes attributes;
    std::vector<glm::vec3> uniqueVerteces;
    bool hasNormals = false;
    bool hasUvs = false;

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

#if 1
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            const float x = attrib.vertices.at(3 * index.vertex_index + 0);
            const float y = attrib.vertices.at(3 * index.vertex_index + 1);
            const float z = attrib.vertices.at(3 * index.vertex_index + 2);

            glm::vec3 coordinate{ x, y, z };
            /*auto it = std::find(uniqueVerteces.begin(), uniqueVerteces.end(), coordinate);
            if (it != uniqueVerteces.end())
            {
                uint32_t pos = uint32_t(it - uniqueVerteces.begin());
                m_indeces.push_back(pos);
            }
            else // New unique vertex */
            {
                data.push_back(x);
                data.push_back(y);
                data.push_back(z);

                if (index.normal_index >= 0)
                {
                    const float nx = attrib.normals[3 * size_t(index.normal_index) + 0];
                    const float ny = attrib.normals[3 * size_t(index.normal_index) + 1];
                    const float nz = attrib.normals[3 * size_t(index.normal_index) + 2];
                    data.push_back(nx);
                    data.push_back(ny);
                    data.push_back(nz);
                    hasNormals = true;
                }

                if (index.texcoord_index >= 0)
                {
                    /*const float tx = attrib.texcoords[2 * size_t(index.texcoord_index) + 0];
                    const float ty = attrib.texcoords[2 * size_t(index.texcoord_index) + 1];
                    data.push_back(tx);
                    data.push_back(ty);
                    hasUvs = true;*/
                }

                m_indeces.push_back((uint32_t)uniqueVerteces.size());
                uniqueVerteces.push_back(coordinate);
            }
        }
    }
#else
    const uint32_t numFloatsPerVertex = 3 + 4; // position + colour
    std::unordered_map<glm::vec3, uint32_t> uniqueVertices;

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            glm::vec3 position{
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2] };

            if (uniqueVertices.count(position) == 0)
            {   
                m_data.push_back(position.x);
                m_data.push_back(position.y);
                m_data.push_back(position.z);

                if (index.normal_index >= 0)
                {
                    /*const float nx = attrib.normals[3 * size_t(index.normal_index) + 0];
                    const float ny = attrib.normals[3 * size_t(index.normal_index) + 1];
                    const float nz = attrib.normals[3 * size_t(index.normal_index) + 2];
                    data.push_back(nx);
                    data.push_back(ny);
                    data.push_back(nz);
                    hasNormals = true;
                    numFloatsPerVertex += 3;*/
                }

                if (index.texcoord_index >= 0)
                {
                    /*const float tx = attrib.texcoords[2 * size_t(index.texcoord_index) + 0];
                    const float ty = attrib.texcoords[2 * size_t(index.texcoord_index) + 1];
                    data.push_back(tx);
                    data.push_back(ty);
                    hasUvs = true;
                    numFloatsPerVertex += 2;*/
                }

                uniqueVertices[position] = static_cast<uint32_t>(m_data.size() / numFloatsPerVertex);
            }

            m_indeces.push_back(uniqueVertices[position]);
        }
    }
#endif

    attributes.push_back(VertexAttribute{ "positionIn", 3 });

    if (hasNormals)
    {
        attributes.push_back(VertexAttribute{ "normalIn", 3 });
    }

    m_verteces = new Verteces{data, attributes};
}
