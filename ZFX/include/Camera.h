#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"


namespace ZFX
{
	class Camera
	{
	public:
		/* pos = camera position
		*  aspect = aspect ratio
		*  fov = field of view
		*  zNear = closest the camera can see ( can't see objects right next to the camera)
		*  zFar = furthest the camera can see (can't see infinitely far objects)
		*/
		Camera(const glm::vec3& pos, const float aspect, const float fov = 70.0f,
			const float zNear = 0.01f, const float zFar = 1000.0f)
		{
			m_perspective = glm::perspective(fov, aspect, zNear, zFar);
			m_position = pos;
			m_forward = glm::vec3(0, 0, -1);
			m_up = glm::vec3(0, 1, 0);
		}

		Camera(const Camera& other) = delete;
		Camera& operator=(const Camera& other) = delete;

		glm::mat4 getViewProjection() const
		{
			return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
		}

	private:
		glm::mat4 m_perspective;
		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
	};
}
