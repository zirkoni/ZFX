#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"


namespace ZFX
{
    class Transform
    {
    public:
        Transform(const glm::vec3& pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
            const glm::vec3& rot = glm::vec3{ 0.0f, 0.0f, 0.0f },
            const glm::vec3& scale = glm::vec3{ 1.0f, 1.0f, 1.0f }) :
            m_pos(pos),
            m_rot(rot),
            m_scale(scale) {}

        glm::mat4 getModel() const
        {
            const glm::mat4 posMatrix = glm::translate(m_pos);
            const glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3{ 1.0f, 0.0f, 0.0f });
            const glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3{ 0.0f, 1.0f, 0.0f });
            const glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3{ 0.0f, 0.0f, 1.0f });
            const glm::mat4 scaleMatrix = glm::scale(m_scale);

            const glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

            return (posMatrix * rotMatrix * scaleMatrix);
        }

        glm::vec3& position() { return m_pos; }
        glm::vec3& rotation() { return m_rot; }
        glm::vec3& scale() { return m_scale; }

        const glm::vec3& position() const { return m_pos; }
        const glm::vec3& rotation() const { return m_rot; }
        const glm::vec3& scale()    const { return m_scale; }

    private:
        glm::vec3 m_pos;
        glm::vec3 m_rot;
        glm::vec3 m_scale;
    };
}
