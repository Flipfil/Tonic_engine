#pragma once

#include <cstdint>

namespace TNK::GRAPHICS
{
	class Mesh
	{
	public:
		Mesh(float* vertex_array, uint32_t vertex_count, uint32_t diensions);
		Mesh(float* vertex_array, uint32_t vertex_count, uint32_t diensions, 
								  uint32_t* element_arr, uint32_t element_count);

		~Mesh();

		void Bind();
		void Unbind();

		inline uint32_t GetVertexCount() const { return m_vertex_count; }
		inline uint32_t GetElementCount() const { return m_element_count; }

	private:
		uint32_t m_vertex_count, m_element_count;
		uint32_t m_VAO, m_EBO;
		uint32_t m_position_VBO;
	};
}