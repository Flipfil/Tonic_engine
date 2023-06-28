#pragma once

#include "external/glm/glm.hpp"

namespace tonic::graphics
{
	class Camera
	{
	public:
		Camera();
		Camera(const Camera& src);

		inline float GetAspectRatio() const { return m_aspect_ratio; }
		inline float GetHeight() const { return m_height; }
		inline float GetNear() const { return m_near; }
		inline float Getfar() const { return m_far; }
		inline void  SetViewMatrix(const glm::mat4& view_matrix) { m_view_matrix = view_matrix; }

		void SetAspectRatio(float aspect_ratio); 
		void SetHeight(float height); 
		void SetNear(float near); 
		void SetFar(float far);

		void SetOrtho(float height, float near = 0.f, float far = 100.f);
		void SetViewMatrix(const glm::vec3& pos, float rot);

		const glm::mat4& GetProjectionMatrix() const { return m_projection_matrix; }
		const glm::mat4& GetViewMatrix() const { return m_view_matrix; }

	private:
		void CalculateProjectionMatrix();

	private:
		float m_aspect_ratio;
		float m_height, m_near, m_far;
		
		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;
	};

}