#include "ZFX.h"
#include <memory>
#include <iostream>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))


std::unique_ptr<ZFX::Mesh> addTriangle()
{
	ZFX::Vertex vertices[] =
	{
		/*                        x       y                red  green  blue  alpha   */
		ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 0.1f } },
		ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 0.0f, 1.0f, 0.0f, 0.5f } },
		ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	unsigned int indeces[] =
	{
		0, 1, 2
	};

	return std::make_unique<ZFX::Mesh>(vertices, ARRAY_SIZE(vertices), indeces, ARRAY_SIZE(indeces));
}

void mainLoop(ZFX::Window& window)
{
	auto triangle = addTriangle();

	ZFX::Shader shader("../../../Shaders/basicShader");

	bool exitRequested = false;
	SDL_Event e;

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

		shader.bind();
		shader.update();

		triangle->draw();

		window.update();

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