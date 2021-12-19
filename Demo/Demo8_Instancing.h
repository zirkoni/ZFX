#pragma once
#include "Demo.h"
#include <cmath>
#include <vector>


// THIS  IS THE WRONG WAY TO DRAW MULTIPLE INSTANCES OF THE SAME MESH!!!!!
class Demo8 : public Demo
{
public:
    Demo8(ZFX::Camera& camera) : Demo{ camera }, m_counter{ 0.0f }
    {
    	float radius = 50.0;
    	float offset = 2.5f;

    	for(unsigned i = 0; i < NUM_OBJECTS; ++i)
    	{
    		auto cube = addCube();

    		float angle = (float)i / (float)NUM_OBJECTS * 360.0f;
    		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;

    		cube->transform.scale() = glm::vec3{ (rand() % 20) / 50.0f + 0.05f };

    		cube->transform.position().x = sin(angle) * radius + displacement;

    		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    		cube->transform.position().y = displacement * 0.4f;

    		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    		cube->transform.position().z = cos(angle) * radius + displacement;

    		cube->transform.position().y -= cube->transform.position().z * 0.4;

    		cube->transform.rotation().x = rand() % 360;
    		cube->transform.rotation().y = rand() % 360;
    		cube->transform.rotation().z = rand() % 360;

    		m_cubes.push_back(std::move(cube));
    	}
    }

    void draw() override
    {
    	for(const auto& c : m_cubes)
    	{
    		c->transform.rotation().z = m_counter;
			c->transform.rotation().x = m_counter;
    		c->draw(m_camera);
    	}

    	m_counter += 0.01f;
    }

private:
    std::unique_ptr<BasicShape> addCube()
	{
		ZFX::Verteces vertices =
		{
			ZFX::VertexData
			{
				-1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
				 1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
				-1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
				-1.0f,  -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
				 1.0f,  -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
				-1.0f,  -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,
				 1.0f,  -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,
				1.0f,  1.0f,  1.0f,     1.0f, 0.0f, 0.0f,
				1.0f,  1.0f, -1.0f,     1.0f, 0.0f, 0.0f,
				1.0f, -1.0f,  1.0f,     1.0f, 0.0f, 0.0f,
				1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 0.0f,
				-1.0f,  1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,
				-1.0f,  1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
				-1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
				 1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
				 1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
				-1.0f,  1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f
			},

			ZFX::VertexAttributes{ {"positionIn", 3}, {"normalIn", 3} }
		};

		ZFX::Indeces indeces =
		{
			8, 10, 9, 9, 10, 11,
			14, 12, 13, 14, 13, 15,
			1, 0, 2, 3, 1, 2,
			4, 5, 6, 5, 7, 6,
			17, 16, 18, 19, 17, 18,
			20, 21, 22, 21, 23, 22
		};

		return std::make_unique<BasicShape>(vertices, indeces, "instancing");
	}

private:
    static constexpr unsigned NUM_OBJECTS = 1000;

    float m_counter;
    std::vector<std::unique_ptr<BasicShape> > m_cubes;
};




