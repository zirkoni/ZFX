#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Vertex.h"
#include <memory>
#include <vector>


namespace ZFX
{
    class ObjectPart
    {
    public:
        ObjectPart(const Vertices& vertices, const Indices& indices);

        Transform& duplicate();
        void duplicate(Transform& transform);

        const Mesh& mesh() const { return *m_mesh.get(); }

        Transform& transform() { return m_transforms.front(); }
        Transform& transform(int idx) { return m_transforms.at(idx); }
        const std::vector<Transform>& transformList() const { return m_transforms; }

    private:
        std::unique_ptr<Mesh> m_mesh;
        std::vector<Transform> m_transforms;
    };

    class Object
    {
    public:
        using VertexList = std::vector<Vertices>;
        using IndexList = std::vector<Indices>;
        using MeshList = std::vector<ObjectPart>;
        using TextureList = std::vector<std::unique_ptr<Texture>>;

        Object() = default;

        void load(const VertexList& vertices, const IndexList& indices, const std::string& shadersFileName);
        void load(const VertexList& vertices, const IndexList& indices, const ShaderSource& shaderSrc);
        void load(const VertexList& vertices, const IndexList& indices, std::shared_ptr<ZFX::Shader> shader);
        void loadTexture(const std::string& textureFile);

        void draw(const ZFX::Camera& camera) const;
        void draw(ZFX::Shader& shader) const;

        Transform& transform() { return m_meshes[0].transform(); }

        MeshList& meshes()      { return m_meshes; }
        Shader& shader()        { return *m_shader.get(); }
        TextureList& textures() { return m_textures; }

    private:
        MeshList m_meshes;
        std::shared_ptr<Shader> m_shader;
        TextureList m_textures;
    };
}
