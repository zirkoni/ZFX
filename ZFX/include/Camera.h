#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"


namespace ZFX
{
    class Camera
    {
        static constexpr float ZOOM_AMOUNT = 1.0f;

    public:
        enum class Direction: unsigned
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };

        /* pos = camera position
         *  aspect = aspect ratio
         *  zNear = closest the camera can see ( can't see objects right next to the camera)
         *  zFar = farthest the camera can see (can't see infinitely far objects)
         */
        Camera(const glm::vec3& pos, const float aspect, const float zNear = 0.01f, const float zFar = 1000.0f)
        {
            m_yaw = -90.0f;
            m_pitch = 0.0f;
            m_zoom = 45.0f;
            m_aspect = aspect;
            m_zNear = zNear;
            m_zFar = zFar;

            m_position = pos;
            m_front = glm::vec3{ 0.0f, 0.0f, -1.0f };
            m_worldUp = glm::vec3{ 0.0f, 1.0f, 0.0f };
            updateCameraVectors();
        }

        glm::mat4 getViewProjection() const
        {
            glm::mat4 view = glm::lookAt(m_position, m_position + m_front, m_up);
            glm::mat4 projection = glm::perspective(glm::radians(m_zoom), m_aspect, m_zNear, m_zFar);
            return projection * view;
        }

        glm::vec3& position() { return m_position; }
        const glm::vec3& position() const { return m_position; }
        const glm::vec3& front() const { return m_front; }

        void move(const Direction dir, const float deltaTime, const float speed = 0.01f)
        {
            float velocity = speed * deltaTime;

            if (dir == Direction::FORWARD)  m_position += m_front * velocity;
            if (dir == Direction::BACKWARD) m_position -= m_front * velocity;
            if (dir == Direction::LEFT)     m_position -= m_right * velocity;
            if (dir == Direction::RIGHT)    m_position += m_right * velocity;
        }

        void turn(float xOffset, float yOffset)
        {
            constexpr float sensitivity = 0.1f; // TODO: deltaTime?

            m_yaw += (xOffset * sensitivity);
            m_pitch += (yOffset * sensitivity);

            if(m_pitch > 89.0f) m_pitch = 89.0f;
            if(m_pitch < -89.0f) m_pitch = -89.0f;

            updateCameraVectors();
        }

        void zoomIn(float amount = ZOOM_AMOUNT)
        {
            zoom(amount);
        }

        void zoomOut(float amount = ZOOM_AMOUNT)
        {
            zoom(-amount);
        }

        void resetZoom()
        {
            m_zoom = 45.0f;
        }

    private:
        void updateCameraVectors()
        {
            glm::vec3 front;
            front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            front.y = sin(glm::radians(m_pitch));
            front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front = glm::normalize(front);
            m_right = glm::normalize(glm::cross(m_front, m_worldUp));
            m_up    = glm::normalize(glm::cross(m_right, m_front));
        }

        void zoom(float amount)
        {
            m_zoom += amount;

            if (m_zoom < 1.0f) m_zoom = 1.0f;
            if (m_zoom > 70.0f) m_zoom = 70.0f;
        }

    private:
        float       m_yaw;
        float       m_pitch;
        float       m_zoom;
        float       m_aspect;
        float       m_zNear;
        float       m_zFar;

        glm::vec3   m_position;
        glm::vec3   m_front;
        glm::vec3   m_up;
        glm::vec3   m_right;
        glm::vec3   m_worldUp;
    };
}

