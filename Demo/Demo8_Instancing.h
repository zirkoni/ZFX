#pragma once
#include "Demo.h"
#include <cmath>
#include <vector>


// THIS  IS THE WRONG WAY TO DRAW MULTIPLE INSTANCES OF THE SAME MESH!!!!!
class Demo8 : public Demo
{

    struct Cube
    {
        static constexpr float RADIUS = 30.0;
        static constexpr float OFFSET = 3.5f;

        Cube(std::unique_ptr<BasicShape> s, unsigned idx): shape{ std::move(s) }
        {
            angle = (float)idx / (float)NUM_OBJECTS * 360.0f;
            xDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            yDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            zDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;

            shape->transform.scale() = glm::vec3{ (rand() % 20) / 50.0f + 0.05f };
            shape->transform.rotation().y = rand() % 360;
        }

        void update(float counter)
        {
            // Move in a circle
            shape->transform.position().x = sin(angle + counter / 10) * RADIUS + xDisplacement;
            shape->transform.position().z = cos(angle + counter / 10) * RADIUS + zDisplacement;
            shape->transform.position().y = (yDisplacement - shape->transform.position().z) * 0.4f;

            // Rotate cube around its center
            shape->transform.rotation().z = counter;
            shape->transform.rotation().x = counter;
        }

        float angle;
        float xDisplacement;
        float yDisplacement;
        float zDisplacement;

        std::unique_ptr<BasicShape> shape;
    };

public:
    Demo8(ZFX::Camera& camera) : Demo{ camera }, m_counter{ 0.0f }
    {
        for(unsigned i = 0; i < NUM_OBJECTS; ++i)
        {
            Cube cube{ addCube(), i };
            m_cubes.push_back(std::move(cube));
        }
    }

    void draw() override
    {
        for(auto& c : m_cubes)
        {
            c.update(m_counter);
            c.shape->draw(m_camera);
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
    std::vector<Cube> m_cubes;
};




