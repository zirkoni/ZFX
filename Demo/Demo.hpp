#pragma once
#include "ZFX.h"


struct BasicShape
{
	BasicShape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces) :
		mesh{ vertices, indeces },
		shader{ "../../../Shaders/basicShader" },
		transform{} {}

	void draw(const ZFX::Camera& camera)
	{
		shader.bind();
		shader.update(transform, camera);
		mesh.draw();
	}

	ZFX::Mesh mesh;
	ZFX::Shader shader;
	ZFX::Transform transform;
};

struct TexturedShape
{
	TexturedShape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces) :
		mesh{ vertices, indeces },
		shader{ "../../../Shaders/basicShaderTexture", true },
		transform{},
		texture{ "../../../Demo/Textures/texture.png" } {}

	void draw(const ZFX::Camera& camera)
	{
		shader.bind();
		shader.update(transform, camera);
		texture.bind(0);
		mesh.draw();
	}

	ZFX::Mesh mesh;
	ZFX::Shader shader;
	ZFX::Transform transform;
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