#pragma once
#include "Demo.h"
#include <cmath>
#include <vector>


#if 1
class Demo8 : public Demo
{
    static constexpr unsigned NUM_OBJECTS = 6200; // Can be increased much higher without drop in frame rate!
    static constexpr float RADIUS = 30.0;
    static constexpr float OFFSET = 3.5f;

    struct Shape
    {
        Shape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces,
                const std::vector<glm::mat4>& instanceMatrix, const ZFX::Uniforms& uniforms) :
            mesh{ vertices, indeces, instanceMatrix },
            shader{ SHADERS_PATH + "instancing", vertices.attributes(), uniforms } {}

        ~Shape() {}

        void draw(const ZFX::Camera& camera)
        {
            shader.bind();
            shader.update(camera, "viewProjection");
            mesh.draw(NUM_OBJECTS);
        }

        ZFX::Mesh mesh;
        ZFX::Shader shader;
    };

public:
    Demo8(ZFX::Camera& camera) : Demo{ camera }
    {
        std::vector<glm::mat4> instanceMatrix;

        for(unsigned i = 0; i < NUM_OBJECTS; ++i)
        {
            ZFX::Transform transform;

            float angle = (float)i / (float)NUM_OBJECTS * 360.0f;
            float xDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            float yDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            float zDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;

            transform.position().x = sin(angle) * RADIUS + xDisplacement;
            transform.position().z = cos(angle) * RADIUS + zDisplacement;
            transform.position().y = (yDisplacement - transform.position().z) * 0.4f;

            transform.scale() = glm::vec3{ (rand() % 20) / 50.0f + 0.05f };
            transform.rotation().y = rand() % 360;
            instanceMatrix.push_back(transform.getModel());
        }

        m_cube = addCube(instanceMatrix);
    }

    void draw() override
    {
        // No movement!
        m_cube->draw(m_camera);
    }

private:
    std::unique_ptr<Shape> addCube(const std::vector<glm::mat4>& instanceMatrix)
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

        return std::make_unique<Shape>(vertices, indeces, instanceMatrix, ZFX::Uniforms{"viewProjection"});
    }

private:
    std::unique_ptr<Shape> m_cube;
};
#else // Each cube has its own draw call => slow.
class Demo8 : public Demo
{
    static constexpr unsigned NUM_OBJECTS = 6200; // Higher than ~this not reaching 60fps anymore (GTX 1660 Super)

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
            update(0.0f);
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
//            c.update(m_counter); // Movement
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

        return std::make_unique<BasicShape>(vertices, indeces, "colour3D_Lighting");
    }

private:
    float m_counter;
    std::vector<Cube> m_cubes;
};
#endif


