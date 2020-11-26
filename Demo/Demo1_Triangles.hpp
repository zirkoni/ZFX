#pragma once
#include "Demo.hpp"


class Demo1 : public Demo
{
public:
	Demo1(ZFX::Camera& camera) : Demo{ camera }
	{
		m_triangle = addTriangle();
		m_texturedTriangle = addTexturedTriangle();
	}

	void draw() override
	{
		m_triangle->transform.position().x = sin(m_counter);
		m_texturedTriangle->transform.position().y = sin(m_counter);
		m_counter += 0.001f;

		m_triangle->draw(m_camera);
		m_texturedTriangle->draw(m_camera);
	}

private:
	std::unique_ptr<BasicShape> addTriangle()
	{
		/* The 3 corners of a triangle (clockwise) */
#if 1
		ZFX::Vertex vertices[] =
		{
			/*                        x       y                red  green  blue  alpha   */
			ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 0.1f } },
			ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 0.0f, 1.0f, 0.0f, 0.5f } },
			ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } }
		};
#else
		/* Counter clockwise => Window.cpp sets clockwise winding => this triangle is not visible */
		ZFX::Vertex vertices[] =
		{
			/*                        x       y                red  green  blue  alpha   */
			ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 0.1f } },
			ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } },
			ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 0.0f, 1.0f, 0.0f, 0.5f } }
		};
#endif

		/* For a triangle these don't really matter. Check out square! */
		uint32_t indeces[] =
		{
			0, 1, 2
		};

		return std::make_unique<BasicShape>(vertices, 3, indeces, 3);
	}

	std::unique_ptr<TexturedShape> addTexturedTriangle()
	{
		/* The 3 corners of a triangle */
		/* Texture coordinates have (0, 0) at the lower left corder and (1, 1) at the upper right corner */
		ZFX::Vertex vertices[] =
		{
			/*                        x       y                red  green  blue  alpha         texture coordinates */
			ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2{ 0.0f, 0.0f } },
			ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2{ 0.5f, 1.0f } },
			ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2{ 1.0f, 0.0f } }
		};

		/* For a triangle these don't really matter. Check out square! */
		uint32_t indeces[] =
		{
			0, 1, 2
		};

		return std::make_unique<TexturedShape>(vertices, 3, indeces, 3);
	}

private:
	float m_counter;
	std::unique_ptr<BasicShape> m_triangle;
	std::unique_ptr<TexturedShape> m_texturedTriangle;
};