#include "../Source/ZFX.h"
#include <memory>


std::unique_ptr<ZFX_2D::Color::Mesh> addTriangle()
{
	ZFX_2D::Color::Vertex vertices[] =
	{
		ZFX_2D::Color::Vertex{ glm::vec2{ -0.5f, -0.5f } },
		ZFX_2D::Color::Vertex{ glm::vec2{  0.0f,  0.5f } },
		ZFX_2D::Color::Vertex{ glm::vec2{  0.5f, -0.5f } }
	};

	unsigned int indeces[] =
	{
		0, 1, 2
	};

	return std::make_unique<ZFX_2D::Color::Mesh>(vertices, sizeof(vertices) / sizeof(vertices[0]),
		indeces, sizeof(indeces) / sizeof(indeces[0]));
}

void mainLoop(ZFX::Window& window)
{
	auto triangle = addTriangle();

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