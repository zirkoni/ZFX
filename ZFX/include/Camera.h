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
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        /* pos = camera position
         *  aspect = aspect ratio
         *  zNear = closest the camera can see ( can't see objects right next to the camera)
         *  zFar = farthest the camera can see (can't see infinitely far objects)
         */
        Camera(const glm::vec3& pos, const float aspect, const float zNear = 0.01f, const float zFar = 1000.0f);

        glm::mat4 getViewProjection() const;
        glm::vec3& position() { return m_position; }
        const glm::vec3& position() const { return m_position; }
        const glm::vec3& front() const { return m_front; }
        void move(const Direction dir, const float deltaTime, const float speed = 0.01f);
        void turn(float xOffset, float yOffset);

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

        float& yaw() { return m_yaw; }
        const float& yaw() const { return m_yaw; }
        float& pitch() { return m_pitch; }
        const float& pitch() const { return m_pitch; }

        void update()
        {
            updateCameraVectors();
        }

        static void resize(float newAspect)
        {
            s_aspect = newAspect;
        }

    private:
        void updateCameraVectors();
        void zoom(float amount);

    private:
        static float s_aspect;
        float m_yaw;
        float m_pitch;
        float m_zoom;
        float m_zNear;
        float m_zFar;

        glm::vec3   m_position;
        glm::vec3   m_front;
        glm::vec3   m_up;
        glm::vec3   m_right;
        glm::vec3   m_worldUp;
    };
}

