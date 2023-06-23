#include "tonic/graphics/camera.h"

#include "external/glm/gtc/matrix_transform.hpp"

namespace tonic::graphics
{
	Camera::Camera()
		: m_projection_matrix(1.f)
		, m_aspect_ratio(16.f / 9.f)
		, m_height(1080.f)
		, m_near(0.f)
		, m_far(100.f)
	{
	}

	Camera::Camera(const Camera& src)
		: m_projection_matrix(src.m_projection_matrix)
		, m_aspect_ratio(src.m_aspect_ratio)
		, m_height(src.m_height)
		, m_near(src.m_near)
		, m_far(src.m_far)
	{
		CalculateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float aspect_ratio)
	{
		if (aspect_ratio != m_aspect_ratio)
		{
			m_aspect_ratio = aspect_ratio;
			CalculateProjectionMatrix();
		}
	}

	void Camera::SetHeight(float height)
	{
		if (m_height != height)
		{
			m_height = height;
			CalculateProjectionMatrix();
		}
	}

	void Camera::SetNear(float near)
	{
		if (m_near != near)
		{
			m_near = near;
			CalculateProjectionMatrix();
		}
	}

	void Camera::SetFar(float far)
	{
		if (m_far != far)
		{
			m_far = far;
			CalculateProjectionMatrix();
		}
	}

	void Camera::SetOrtho(float height, float near, float far)
	{
		bool is_different = false;
		if (m_height != height)
		{
			m_height = height;
			is_different = true;
		}

		if (m_near != near)
		{
			m_near = near;
			is_different = true;
		}

		if (m_far != far)
		{
			m_far = far;
			is_different = true;
		}

		if (is_different) CalculateProjectionMatrix();
	}

	void Camera::CalculateProjectionMatrix()
	{
		float half_width = m_height * m_aspect_ratio * 0.5f;
		float half_height = m_height * 0.5f;
		m_projection_matrix = glm::ortho(-half_width, half_width, -half_height, half_height, m_near, m_far);
	}
}