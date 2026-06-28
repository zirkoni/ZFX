#pragma once
#include "Demo6A_BasicLighting.h"
#include <memory>
#include <vector>

//#define LOW_PERF_GPU

class Demo8 : public Demo
{
#ifdef LOW_PERF_GPU
    static constexpr unsigned NUM_OBJECTS = 5000;
#else
    // Can be increased much higher without dropping below 60 fps!
    // My Nvidia GTX 1660 Super can handle 2M cubes.
    static constexpr unsigned NUM_OBJECTS = 42000;
#endif
    static constexpr float RADIUS = 30.0f;
    static constexpr float OFFSET = 3.5f;

    struct Shape
    {
        Shape(const ZFX::Verteces& vertices, const ZFX::Indeces& indeces,
                const std::vector<glm::mat4>& instanceMatrix) :
            mesh{ vertices, indeces, instanceMatrix },
            shader{ {SHADERS_PATH + "instancing.vs", SHADERS_PATH + "instancing.fs"} }
        {}

        void draw(const ZFX::Camera& camera)
        {
            shader.bind();
            shader.update(camera, "u_viewProjection");
            mesh.draw(NUM_OBJECTS);
        }

        ZFX::Mesh mesh;
        ZFX::Shader shader;
    };

public:
    Demo8(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo8 - Instancing" }
    {
        srand(SDL_GetTicks());

        std::vector<glm::vec4> animParams0;
        std::vector<glm::vec2> animParams1;

        for(unsigned i = 0; i < NUM_OBJECTS; ++i)
        {
            float angle = (float)i / (float)NUM_OBJECTS * 360.0f;
            float xDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            float yDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            float zDisplacement = (rand() % (int)(2 * OFFSET * 100)) / 100.0f - OFFSET;
            float scale = (rand() % 20) / 50.0f + 0.05f;
            float yRot = rand() % 360;

            animParams0.push_back(glm::vec4(xDisplacement, yDisplacement, zDisplacement, angle));
            animParams1.push_back(glm::vec2(scale, yRot));
        }

        m_instanceMatrix.resize(NUM_OBJECTS, glm::mat4(1.0f));

        m_paramsSsbo = std::make_unique<ZFX::SSBO>(animParams0, 1);
        m_scalesSsbo = std::make_unique<ZFX::SSBO>(animParams1, 2);

        m_cube = addCube();
        m_meshInstanceBuffer = m_cube->mesh.getInstanceBufferID();

        createComputeShader();
        onEntry();
    }

    void update() override
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_meshInstanceBuffer);

        m_computeShader->bind();
        m_computeShader->setUniform("u_counter", m_counter);
        // Each group has 256 threads (local_size_x = 256), ceiling division to calculate num. of work groups
        m_computeShader->compute((NUM_OBJECTS + 255) / 256);

        m_counter += 0.01f;
    }

    void draw() override
    {
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
        return std::make_unique<Shape>(cubeVertecesWithNormals(), cubeIndeces(), m_instanceMatrix);
    }

    void createComputeShader()
    {
        // Note: Compute shader requires OpenGL version 4.3+
        std::string computeSource = R"(
#version 430

layout(std430, binding = 0) buffer InstanceData
{
    mat4 modelMatrices[];
};

layout(std430, binding = 1) buffer AnimationParams0
{
    vec4 params0[];
};

layout(std430, binding = 2) buffer AnimationParams1
{
    vec2 params1[];
};

uniform float u_counter;
uniform float u_radius = 30.0;

layout(local_size_x = 256) in;

void main()
{
    uint idx = gl_GlobalInvocationID.x;

    vec4 param0 = params0[idx];
    float xDisp = param0.x;
    float yDisp = param0.y;
    float zDisp = param0.z;
    float angle = param0.w;

    float posX = sin(angle + u_counter / 10.0) * u_radius + xDisp;
    float posZ = cos(angle + u_counter / 10.0) * u_radius + zDisp;
    float posY = (yDisp - posZ) * 0.4;

    vec2 param1 = params1[idx];
    float rotX = u_counter;
    float rotZ = u_counter;
    float rotY = param1.y;
    float scale = param1.x;

    // Identity
    mat4 M = mat4(1.0);

    // Translate
    M[3] = vec4(posX, posY, posZ, 1.0);

    // Rotate around Z - multiply from right
    float cz = cos(rotZ);
    float sz = sin(rotZ);
    mat4 Rz = mat4(cz, sz, 0, 0, -sz, cz, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    M = M * Rz;

    // Rotate around X - multiply from right
    float cx = cos(rotX);
    float sx = sin(rotX);
    mat4 Rx = mat4(1, 0, 0, 0, 0, cx, sx, 0, 0, -sx, cx, 0, 0, 0, 0, 1);
    M = M * Rx;

    // Rotate around Y - multiply from right
    float cy = cos(rotY);
    float sy = sin(rotY);
    mat4 Ry = mat4(cy, 0, -sy, 0, 0, 1, 0, 0, sy, 0, cy, 0, 0, 0, 0, 1);
    M = M * Ry;

    // Scale
    M[0] *= scale;
    M[1] *= scale;
    M[2] *= scale;

    modelMatrices[idx] = M;
}
        )";

        m_computeShader = std::make_unique<ZFX::ComputeShader>(computeSource);
    }

private:
    std::unique_ptr<Shape> m_cube;
    std::vector<glm::mat4> m_instanceMatrix;
    std::unique_ptr<ZFX::SSBO> m_paramsSsbo;
    std::unique_ptr<ZFX::SSBO> m_scalesSsbo;
    std::unique_ptr<ZFX::ComputeShader> m_computeShader;
    GLuint m_meshInstanceBuffer = 0;
};
