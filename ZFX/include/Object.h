#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include <memory>
#include <vector>


namespace ZFX
{
    class Object
    {
    public:
        Object();

        void load(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces, const std::string shaderName);
        void loadTexture(const std::string& textureFile);
        Transform& duplicate();
        void duplicate(Transform& transform);

        void draw(const ZFX::Camera& camera);

        Transform& transform() { return m_transforms[0]; }
        Transform& transform(int idx) { return m_transforms[idx]; }

        Shader& shader() { return *m_shader.get(); }

    private:
        std::unique_ptr<Mesh> m_mesh;
        std::unique_ptr<Shader> m_shader;
        std::vector<std::unique_ptr<Texture> > m_textures;
        std::vector<Transform> m_transforms;
    };
}