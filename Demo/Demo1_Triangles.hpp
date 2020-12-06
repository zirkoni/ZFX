#pragma once
#include "Demo.hpp"
#include <vector>

#include <iostream>


class Demo1 : public Demo
{
    static constexpr uint32_t NUM_TRIANGLES = 5;

public:
    Demo1(ZFX::Camera& camera) : Demo{ camera }
    {
        for (uint32_t i = 0; i < NUM_TRIANGLES; ++i)
        {
            /* All objects should be usually created around the origin (0,0,0) */
            auto triangle = addTriangle();

            // Their positions are set with transform
            triangle->transform.position().x -= ((float)i - (NUM_TRIANGLES - 1) / 2.0f) * 0.2f;

            m_triangles.push_back(std::move(triangle));
        }

        m_texturedTriangle = addTexturedTriangle();
    }

    void draw() override
    {
        m_triangles.back()->transform.position().x = sin(m_counter);
        m_texturedTriangle->transform.position().y = sin(m_counter);
        m_counter += 0.001f;

        /* Draw opaque triangle 1st */
        m_texturedTriangle->draw(m_camera);

        for (const auto& t : m_triangles)
        {
            t->draw(m_camera);
        }
    }

private:
    std::unique_ptr<BasicShape> addTriangle()
    {
        /* The 3 corners of a triangle (clockwise) */
#if 1
        ZFX::Verteces vertices =
        {
            /*                        x       y                red  green  blue  alpha   */
            ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 0.1f } },
            ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 0.0f, 1.0f, 0.0f, 0.5f } },
            ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } }
        };
#else
        /* Counter clockwise => Window.cpp sets clockwise winding => this triangle is not visible */
        ZFX::Verteces vertices =
        {
            /*                        x       y                red  green  blue  alpha   */
            ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 0.1f } },
            ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } },
            ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 0.0f, 1.0f, 0.0f, 0.5f } }
        };
#endif

        /* For a triangle these don't really matter. Check out square! */
        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        return std::make_unique<BasicShape>(vertices, indeces);
    }

    std::unique_ptr<TexturedShape> addTexturedTriangle()
    {
        /* The 3 corners of a triangle */
        /* Texture coordinates have (0, 0) at the lower left corder and (1, 1) at the upper right corner */
        ZFX::Verteces vertices =
        {
            /*                        x       y                red  green  blue  alpha         texture coordinates */
            ZFX::Vertex{ glm::vec2{ -0.5f, -0.5f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2{ 0.0f, 0.0f } },
            ZFX::Vertex{ glm::vec2{  0.0f,  0.5f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2{ 0.5f, 1.0f } },
            ZFX::Vertex{ glm::vec2{  0.5f, -0.5f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2{ 1.0f, 0.0f } }
        };

        /* For a triangle these don't really matter. Check out square! */
        ZFX::Indeces indeces =
        {
            0, 1, 2
        };

        return std::make_unique<TexturedShape>(vertices, indeces);
    }

private:
    float m_counter;
    std::vector<std::unique_ptr<BasicShape> > m_triangles;
    std::unique_ptr<TexturedShape> m_texturedTriangle;
};