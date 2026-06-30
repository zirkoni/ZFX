#include "Mesh.h"
#if !defined(ZFX_NO_OBJECT_LOADER)
#define TINYOBJLOADER_IMPLEMENTATION

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

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    //auto& materials = reader.GetMaterials();

    for (size_t s = 0; s < shapes.size(); s++)
    {
        VertexData data;
        AttributeSizes attributes;
        Indices indices;
        bool hasNormals = false;
        bool hasUvs = false;

        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                data.push_back(vx);
                data.push_back(vy);
                data.push_back(vz);
                indices.push_back(index_offset + v);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];

                    data.push_back(nx);
                    data.push_back(ny);
                    data.push_back(nz);
                    hasNormals = true;
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];

                    data.push_back(tx);
                    data.push_back(ty);
                    hasUvs = true;
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }

            index_offset += fv;
        }

        attributes.push_back(3);

        if (hasNormals)
        {
            attributes.push_back(3);
        }

        if (hasUvs)
        {
            attributes.push_back(2);
        }

        m_vertices.emplace_back(data, attributes);
        m_indices.emplace_back(indices);
    }
}
#endif
