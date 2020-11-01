#include "ZFX.h"
#include <memory>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))


std::unique_ptr<ZFX::Mesh> addTriangle()
{
	ZFX::Vertex vertices[] =
	{
		ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f } },
		ZFX::Vertex{ glm::vec2{  0.0f,  0.5f } },
		ZFX::Vertex{ glm::vec2{  0.5f, -0.5f } }
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
	ZFX::Window window{ 800, 600, "Demo" };
	mainLoop(window);

	return 0;
}