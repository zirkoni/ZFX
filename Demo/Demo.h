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

    void draw(const ZFX::Camera& camera, ZFX::Texture* texture2)
	{
		shader.bind();
		shader.update(transform, camera);
		texture.bind(0);
		texture2->bind(1);
		mesh.draw();
	}

    ZFX::Texture texture;
};


class Demo
{
public:
    Demo(ZFX::Camera& camera, const std::string& name): m_counter{ 0.0f }, m_camera{ camera }, m_name{ name } {}
    virtual ~Demo() {}

    //virtual void update(float deltaTime) = 0; // TODO
    virtual void draw() = 0;

    const std::string& name() { return m_name; }

protected:
    float m_counter;
    ZFX::Camera& m_camera;
    std::string m_name;
};
