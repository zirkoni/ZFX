#include "Camera.h"


ZFX::Camera::Camera(const glm::vec3& pos, const float aspect, const float zNear, const float zFar)
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

glm::mat4 ZFX::Camera::getView() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 ZFX::Camera::getProjection() const
{
    return glm::perspective(glm::radians(m_zoom), m_aspect, m_zNear, m_zFar);
}

glm::mat4 ZFX::Camera::getViewProjection() const
{
    return getProjection() * getView();
}

void ZFX::Camera::move(const Direction dir, const float deltaTime, const float speed)
{
    float velocity = speed * deltaTime;

    if (dir == Direction::FORWARD)  m_position += m_front * velocity;
    if (dir == Direction::BACKWARD) m_position -= m_front * velocity;
    if (dir == Direction::UP)       m_position += m_up    * velocity;
    if (dir == Direction::DOWN)     m_position -= m_up    * velocity;
    if (dir == Direction::LEFT)     m_position -= m_right * velocity;
    if (dir == Direction::RIGHT)    m_position += m_right * velocity;
}

void ZFX::Camera::turn(float xOffset, float yOffset)
{
    constexpr float sensitivity = 0.1f; // TODO: deltaTime?

    m_yaw += (xOffset * sensitivity);
    m_pitch += (yOffset * sensitivity);

    if(m_pitch > 89.0f) m_pitch = 89.0f;
    if(m_pitch < -89.0f) m_pitch = -89.0f;

    updateCameraVectors();
}

void ZFX::Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void ZFX::Camera::zoom(float amount)
{
    m_zoom += amount;

    if (m_zoom < 1.0f) m_zoom = 1.0f;
    if (m_zoom > 70.0f) m_zoom = 70.0f;
}

