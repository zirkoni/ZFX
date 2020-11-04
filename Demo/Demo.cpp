#include "ZFX.h"
#include <memory>
#include <iostream>


struct BasicShape
{
	BasicShape(ZFX::Vertex* vertices, const uint32_t numVertices, uint32_t* indeces, const uint32_t numIndeces) :
		mesh{ vertices, numVertices, indeces, numIndeces },
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

std::unique_ptr<BasicShape> addTriangle()
{
	/* The 3 corners of a triangle */
	ZFX::Vertex vertices[] =
	{
		/*                        x       y                red  green  blue  alpha   */
		ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 0.1f } },
		ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 0.0f, 1.0f, 0.0f, 0.5f } },
		ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	/* For a triangle these don't really matter. Check out square! */
	uint32_t indeces[] =
	{
		0, 1, 2
	};

	return std::make_unique<BasicShape>(vertices, 3, indeces, 3);
}

void mainLoop(ZFX::Window& window)
{
	auto triangle = addTriangle();

	ZFX::Camera camera{ glm::vec3{0.0f, 0.0f, 3.0f}, window.aspectRatio() };

	bool exitRequested = false;
	SDL_Event e;

	float counter = 0.0f;
	while (!exitRequested)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				exitRequested = true;
			}
		}

		window.clear(0.0f, 0.0f, 0.0f, 1.0f);

		triangle->transform.position().x = sin(counter);
		triangle->draw(camera);

		window.update();

		counter += 0.001f;
		SDL_Delay(1);
	}
}

int main(int argc, char* argv[])
{
	try
	{
		ZFX::Window window{ 800, 600, "Demo" };
		mainLoop(window);
	}
	catch (const std::runtime_error& error) // TODO: maybe typedef type to e.g. ZFX::Exception?
	{
		std::cerr << "Runtime error:\n" << error.what() << "\n";
	}

	return 0;
}