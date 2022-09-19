#pragma once
#include "Demo.h"
#include <cmath>
#include <vector>


#if 1
class Demo8 : public Demo
{
    static constexpr unsigned NUM_OBJECTS = 42000; // Can be increased much higher without dropping below 60 fps!
    static constexpr float RADIUS = 30.0f;
    static constexpr float OFFSET = 3.5f;

    struct Shape
    {
        Shape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces,
                const std::vector<glm::mat4>& instanceMatrix) :
            mesh{ vertices, indeces, instanceMatrix },
            shader{ {SHADERS_PATH + "instancing.vs", SHADERS_PATH + "instancing.fs"} } {}

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

    struct InstanceTransform
    {
        float angle;
        float xDisplacement;
        float yDisplacement;
        float zDisplacement;
        ZFX::Transform transform;
    };

public:
    Demo8(ZFX::Camera& camera) : Demo{ camera, "Demo8" }
    {
        srand(SDL_GetTicks());

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
            m_instanceMatrix.push_back(transform.getModel());
            m_transforms.push_back({angle, xDisplacement, yDisplacement, zDisplacement, transform});
        }

        m_cube = addCube();
    }

    void update()
    {
        for (unsigned i = 0; i < NUM_OBJECTS; ++i)
        {
            auto& t = m_transforms.at(i);
            auto& model = m_instanceMatrix.at(i);

            // Move in a circle
            t.transform.position().x = sin(t.angle + m_counter / 10) * RADIUS + t.xDisplacement;
            t.transform.position().z = cos(t.angle + m_counter / 10) * RADIUS + t.zDisplacement;
            t.transform.position().y = (t.yDisplacement - t.transform.position().z) * 0.4f;

            // Rotate cube around its center
            t.transform.rotation().z = m_counter;
            t.transform.rotation().x = m_counter;

            model = t.transform.getModel();
        }

        m_counter += 0.01f;
        m_cube->mesh.updateModels(m_instanceMatrix);
    }

    void draw() override
    {
        update();
        m_cube->draw(m_camera);
    }

    void onEntry() override
    {
        Demo::onEntry();
        m_camera.position().z = 80.0f;
    }

private:
    std::unique_ptr<Shape> addCube()
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

            ZFX::AttributeSizes{3, 3}
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

        return std::make_unique<Shape>(vertices, indeces, m_instanceMatrix);
    }

private:
    std::unique_ptr<Shape> m_cube;
    std::vector<InstanceTransform> m_transforms;
    std::vector<glm::mat4> m_instanceMatrix;
};
#else // Each cube has its own draw call => slow.
class Demo8 : public Demo
{
    static constexpr unsigned NUM_OBJECTS = 42000; // Higher than ~this not reaching 60fps anymore (GTX 1660 Super)

    struct CubeInstance
    {
        static constexpr float RADIUS = 30.0;
        static constexpr float OFFSET = 3.5f;

        CubeInstance(ZFX::Transform& transform, unsigned idx)
        {
            angle = (float)idx / (float)NUM_OBJECTS * 360.0f;
            xDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            yDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            zDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;

            transform.scale() = glm::vec3{ (rand() % 20) / 50.0f + 0.05f };
            transform.rotation().y = rand() % 360;
            update(transform, 0.0f);
        }

        void update(ZFX::Transform& transform, float counter)
        {
            // Move in a circle
            transform.position().x = sin(angle + counter / 10) * RADIUS + xDisplacement;
            transform.position().z = cos(angle + counter / 10) * RADIUS + zDisplacement;
            transform.position().y = (yDisplacement - transform.position().z) * 0.4f;

            // Rotate cube around its center
            transform.rotation().z = counter;
            transform.rotation().x = counter;
        }

        float angle;
        float xDisplacement;
        float yDisplacement;
        float zDisplacement;
    };

public:
    Demo8(ZFX::Camera& camera) : Demo{ camera, "Demo8" }
    {
        addCube();

        for(unsigned i = 0; i < NUM_OBJECTS; ++i)
        {
            m_transformations.emplace_back( CubeInstance{m_cube.transform(i), i} );

            if(i < NUM_OBJECTS - 1) // Do not  duplicate on last loop iteration
            {
                m_cube.duplicate();
            }
        }
    }

    void draw() override
    {
        for(int i = 0; i < m_transformations.size(); ++i)
        {
            m_transformations[i].update(m_cube.transform(i), m_counter); // Movement
        }

        m_cube.draw(m_camera); // Here draw is called for each Transform
        m_counter += 0.01f;
    }

    void onEntry() override
    {
        m_camera.position().z = 80.0f;
        m_camera.resetZoom();
    }

private:
    void addCube()
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

            ZFX::AttributeSizes{3, 3}
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

        m_cube.load(vertices, indeces, SHADERS_PATH + "colour3D_Lighting");
    }

private:
    ZFX::Object m_cube;
    std::vector<CubeInstance> m_transformations;
};
#endif


