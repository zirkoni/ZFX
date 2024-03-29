#include "Object.h"


ZFX::Object::Object()
{
}

void ZFX::Object::load(const ZFX::Verteces &vertices,
        const ZFX::Indeces &indeces, const std::string &shadersFileName)
{
    ShaderSource src{ shadersFileName + ".vs", shadersFileName + ".fs" };
    load(vertices, indeces, src);
}

void ZFX::Object::load(const ZFX::Verteces &vertices, const ZFX::Indeces &indeces,
        std::shared_ptr<ZFX::Shader> shader)
{
    m_mesh = std::make_unique<Mesh>(vertices, indeces);
    m_shader = shader;
    m_transforms.push_back( Transform{} );
}

void ZFX::Object::load(const ZFX::Verteces &vertices, const ZFX::Indeces &indeces,
        const ShaderSource& shaderSrc)
{
    m_mesh = std::make_unique<Mesh>(vertices, indeces);
    m_shader = std::make_shared<Shader>(shaderSrc);
    m_transforms.push_back( Transform{} );
}

void ZFX::Object::loadTexture(const std::string &textureFile)
{
    auto texture = std::make_unique<Texture>(textureFile);
    m_textures.push_back(std::move(texture));
}

ZFX::Transform& ZFX::Object::duplicate()
{
    auto& copy = m_transforms[0];
    m_transforms.push_back(copy);
    return m_transforms.back();
}

void ZFX::Object::duplicate(Transform &transform)
{
    m_transforms.push_back(transform);
}

void ZFX::Object::draw(const ZFX::Camera &camera)
{
    m_shader->bind();

    for(uint32_t i = 0; i < m_textures.size(); ++i)
    {
        m_textures[i]->bind(i);
    }

    for(const auto& t : m_transforms)
    {
        m_shader->update(t, camera);
        m_mesh->draw();
    }
}

void ZFX::Object::draw(ZFX::Shader &shader)
{
    shader.bind();

    for(const auto& t : m_transforms)
    {
        shader.update(t.getModel());
        m_mesh->draw();
    }
}
