#pragma once
#include "Demo.h"


class Demo9 : public Demo
{
public:
    Demo9(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo9 - Mouse" }
    {
        // Add squares to different positions and scale
        addSquare(0.0f, 0.0f, -0.2f, 0.8f);
        addSquare(-1.0f, 0.5f, 0.5f, 0.2f);
        addSquare(-1.0f, -1.0f, 0.0f, 0.3f);
        addSquare(1.0f, 1.0f, 0.0f, 0.4f);
        addSquare(1.0f, -1.0f, -0.5f, 0.5f);

        // Collision detection works with rotation as well
        m_squares.front().transform().rotation().z = 0.5;
    }

    void update() override
    {
        updateMousePosition();

        for (size_t i = 0; i < m_squares.size(); ++i)
        {
            const bool oldState = m_collisionStates[i];
            m_collisionStates[i] = isMouseInsideSquare(m_squares[i]);

            // Update only on state change
            if (oldState != m_collisionStates[i])
            {
                if (m_collisionStates[i])
                {
                    handleCollision(m_squares[i]);
                } else
                {
                    handleNoCollision(m_squares[i]);
                }
            }
        }
    }

    void draw() override
    {
        for (const auto& square : m_squares)
        {
            square.draw(m_camera);
        }
    }

private:

    bool isMouseInsideSquare(ZFX::Object& square)
    {
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        float winX = static_cast<float>(m_mx);
        float winY = static_cast<float>(viewport[3] - m_my);

        const glm::mat4 view = m_camera.getView();
        const glm::mat4 projection = m_camera.getProjection();
        const glm::mat4 model = square.transform().getModel();

        glm::vec3 nearPoint = glm::unProject(
            glm::vec3(winX, winY, 0.0f),
            view,
            projection,
            glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

        glm::vec3 farPoint = glm::unProject(
            glm::vec3(winX, winY, 1.0f),
            view,
            projection,
            glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

        glm::vec3 rayOrigin = nearPoint;
        glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

        glm::mat4 invModel = glm::inverse(model);

        glm::vec3 localOrigin =
            glm::vec3(invModel * glm::vec4(rayOrigin, 1.0f));

        glm::vec3 localDir =
            glm::normalize(
                glm::vec3(invModel * glm::vec4(rayDir, 0.0f)));

        return RaySquareIntersect(
            localOrigin,
            localDir);
    }

    bool RaySquareIntersect(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDir)
    {
        constexpr float EPSILON = 0.0001f;

        // Square normal (square lies on z=0)
        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        float denom = glm::dot(normal, rayDir);

        if (std::abs(denom) < EPSILON)
        {
            // Ray parallel to plane
            return false;
        }

        // Solve intersection
        float t = -rayOrigin.z / rayDir.z;

        if (t < EPSILON)
        {
            // Behind camera
            return false;
        }

        glm::vec3 hitPoint = rayOrigin + rayDir * t;

        // Check if hit point is inside the square
        return hitPoint.x >= -0.5f &&
        hitPoint.x <=  0.5f &&
        hitPoint.y >= -0.5f &&
        hitPoint.y <=  0.5f;
    }

    void addSquare(float x, float y, float z, float scale)
    {
        ZFX::Vertices vertices =
        {
            ZFX::VertexData
            {
            //    x      y   
                -0.5f, -0.5f,
                -0.5f,  0.5f,
                 0.5f,  0.5f,
                 0.5f, -0.5f
            },

            ZFX::AttributeSizes{2}
        };

        ZFX::Indices indices =
        {
            0, 2, 1,
            0, 3, 2
        };

        ZFX::Object square;
        square.load({vertices}, {indices}, SHADERS_PATH + "colour2");
        square.transform().position() = glm::vec3(x, y, z);
        square.transform().scale() = glm::vec3(scale);
        handleNoCollision(square); // Need to set the colour
        m_collisionStates.push_back(false);
        m_squares.push_back(std::move(square));
    }

    void handleCollision(ZFX::Object& square)
    {
        glm::vec4 colour{1.0f, 0.0f, 0.0f, 1.0f}; // Red
        square.shader().bind();
        square.shader().setUniform("u_colour", colour);
    }

    void handleNoCollision(ZFX::Object& square)
    {
        glm::vec4 colour{0.0f, 1.0f, 0.0f, 1.0f}; // Green
        square.shader().bind();
        square.shader().setUniform("u_colour", colour);
    }

    void updateMousePosition()
    {
        SDL_GetMouseState(&m_mx, &m_my);
    }


    std::vector<ZFX::Object> m_squares;
    std::vector<bool> m_collisionStates;
    int m_mx;
    int m_my;
};
