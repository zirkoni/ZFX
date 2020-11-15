#include "Demo.hpp"


class Demo2 : public Demo
{
public:
	Demo2(ZFX::Camera& camera) : Demo{ camera }
	{
		m_square = addSquare();
	}

	void draw() override
	{
		m_square->transform.rotation().z = m_counter;
		m_counter += 0.001f;

		m_square->draw(m_camera);
	}

private:
	std::unique_ptr<BasicShape> addSquare()
	{
		/* The 4 corners of a square, starting from left lower corner in clockwise order */
		ZFX::Vertex vertices[] =
		{
			/*                        x       y                red  green  blue  alpha   */
			ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 0.2f, 0.2f, 0.2f, 1.0f } },
			ZFX::Vertex{ glm::vec2{ -0.5f,  0.5f }, glm::vec4{ 0.2f, 0.2f, 0.2f, 1.0f } },
			ZFX::Vertex{ glm::vec2{  0.5f,  0.5f }, glm::vec4{ 0.2f, 0.2f, 0.2f, 1.0f } },
			ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 0.8f, 0.8f, 0.8f, 1.0f } }
		};

		/* Now we use indeces to define our shape (square is made of 2 triangles that have 2 common verteces) */
		uint32_t indeces[] =
		{
			0, 1, 2, // 1st triangle: (-0.5, -0.5), (-0.5, 0.5), (0.5, 0.5)
			0, 2, 3  // 2nd triangle: (-0.5, -0.5), (0.5, 0.5), (0.5, -0.5)
		};

		return std::make_unique<BasicShape>(vertices, 4, indeces, 6);
	}

private:
	float m_counter;
	std::unique_ptr<BasicShape> m_square;
};
