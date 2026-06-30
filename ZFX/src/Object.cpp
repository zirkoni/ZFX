#include "Object.h"
#include "Shader.h"
#include <memory>


ZFX::ObjectPart::ObjectPart(const Vertices& vertices, const Indices& indices)
{
    m_mesh = std::make_unique<Mesh>(vertices, indices);
    m_instances.push_back( Transform{} );
}

ZFX::Transform& ZFX::ObjectPart::addInstance()
{
    auto& copy = m_instances[0];
    m_instances.push_back(copy);
    return m_instances.back();
}

void ZFX::ObjectPart::addInstance(const Transform& transform)
{
    m_instances.push_back(transform);
}

//////////////////////////////////////////////////////////////////////////
void ZFX::Object::load(
        const VertexList& vertices,
        const IndexList& indices,
        const std::string& shadersFileName)
{
    ShaderSource src{ shadersFileName + ".vs", shadersFileName + ".fs" };
    load(vertices, indices, src);
}

void ZFX::Object::load(
        const VertexList& vertices,
        const IndexList& indices,
        std::shared_ptr<ZFX::Shader> shader)
{
    m_shader = shader;

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        m_meshes.emplace_back(vertices[i], indices[i]);
    }
}

void ZFX::Object::load(
        const VertexList& vertices,
        const IndexList& indices,
        const ShaderSource& shaderSrc)
{
    load(vertices, indices, std::make_shared<ZFX::Shader>(shaderSrc));
}

void ZFX::Object::loadTexture(const std::string& textureFile)
{
    auto texture = std::make_unique<Texture>(textureFile);
    m_textures.push_back(std::move(texture));
}

void ZFX::Object::draw(const ZFX::Camera& camera) const
{
    m_shader->bind();

    for(uint32_t i = 0; i < m_textures.size(); ++i)
    {
        m_textures[i]->bind(i);
    }

    for(const auto& mesh : m_meshes)
    {
        for(const auto& t : mesh.instances())
        {
            m_shader->update(t, camera);
            mesh.mesh().draw();
        }
    }
}

void ZFX::Object::draw(ZFX::Shader& shader) const
{
    shader.bind();

    for(const auto& mesh : m_meshes)
    {
        for(const auto& t : mesh.instances())
        {
            shader.update(t.getModel());
            mesh.mesh().draw();
        }
    }
}
