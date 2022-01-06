#pragma once
#include "ZFX.h"
#include "Constants.h"
#include <memory>


struct BasicShape
{
    BasicShape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces, const std::string shaderName = "colour") :
        mesh{ vertices, indeces },
        shader{ SHADERS_PATH + shaderName },
        transform{} {}

    BasicShape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces, const ZFX::Uniforms& uniforms,
        const std::string shaderName = "colour2" ) :
        mesh{ vertices, indeces },
        shader{ SHADERS_PATH + shaderName, uniforms },
        transform{} {}
    
    virtual ~BasicShape() {}

    virtual void draw(const ZFX::Camera& camera)
    {
        shader.bind();
        shader.update(transform, camera);
        mesh.draw();
    }

    ZFX::Mesh mesh;
    ZFX::Shader shader;
    ZFX::Transform transform;
};

struct TexturedShape: public BasicShape
{
    TexturedShape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces, const std::string& textureFile,
        const std::string& shaderName = "texture") :
            BasicShape{ vertices, indeces, shaderName },
            texture{ TEXTURES_PATH + textureFile } {}

    void draw(const ZFX::Camera& camera) override
    {
        shader.bind();
        shader.update(transform, camera);
        texture.bind(0);
        mesh.draw();
    }

    ZFX::Texture texture;
};


class Demo
{
public:
    Demo(ZFX::Camera& camera) : m_camera{ camera } {}
    virtual ~Demo() {}

    virtual void draw() = 0;

protected:
    ZFX::Camera& m_camera;
};
