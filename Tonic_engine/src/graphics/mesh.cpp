#include "tonic/graphics/mesh.h"
#include "tonic/graphics/helpers.h"

#include "glad/glad.h"

namespace tonic::graphics
{
	Mesh::Mesh(const float* vertex_array, uint32_t vertex_count, uint32_t dimensions)
		:m_vertex_count(vertex_count)
		,m_element_count(0)
		,m_EBO(0)
	{
		glGenVertexArrays(1, &m_VAO); TONIC_CHECK_GL_ERROR;
		glBindVertexArray(m_VAO); TONIC_CHECK_GL_ERROR;

		glGenBuffers(1, &m_position_VBO); TONIC_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, m_position_VBO); TONIC_CHECK_GL_ERROR;
		glBufferData(GL_ARRAY_BUFFER, m_vertex_count * dimensions * sizeof(float), vertex_array, GL_STATIC_DRAW); TONIC_CHECK_GL_ERROR;

		glEnableVertexAttribArray(0); TONIC_CHECK_GL_ERROR; 
		glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, 0, 0); TONIC_CHECK_GL_ERROR; 
		glDisableVertexAttribArray(0); TONIC_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, 0); TONIC_CHECK_GL_ERROR;
		

		glBindVertexArray(0); TONIC_CHECK_GL_ERROR;
	}

	Mesh::Mesh(const float* vertex_array, uint32_t vertex_count, uint32_t dimensions, const uint32_t* element_arr, uint32_t element_count)
		:Mesh(vertex_array, vertex_count, dimensions)
	{
		m_element_count = element_count;
		glBindVertexArray(m_VAO); TONIC_CHECK_GL_ERROR;

		glGenBuffers(1, &m_EBO); TONIC_CHECK_GL_ERROR;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); TONIC_CHECK_GL_ERROR;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_element_count * sizeof(float), element_arr, GL_STATIC_DRAW); TONIC_CHECK_GL_ERROR;


		glBindVertexArray(0); TONIC_CHECK_GL_ERROR;
	}

	Mesh::Mesh(const float* vertex_array, uint32_t vertex_count, uint32_t dimensions, const float* texture_coordinates, const uint32_t* element_arr, uint32_t element_count)
		:Mesh(vertex_array, vertex_count, dimensions, element_arr, element_count)
	{
		glBindVertexArray(m_VAO); TONIC_CHECK_GL_ERROR;

		glGenBuffers(1, &m_tex_coords_VBO); TONIC_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, m_tex_coords_VBO); TONIC_CHECK_GL_ERROR;
		glBufferData(GL_ARRAY_BUFFER, vertex_count * 2 * sizeof(float), texture_coordinates, GL_STATIC_DRAW); TONIC_CHECK_GL_ERROR;

		glEnableVertexAttribArray(1); TONIC_CHECK_GL_ERROR;
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); TONIC_CHECK_GL_ERROR;
		glDisableVertexAttribArray(0); TONIC_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, 0); TONIC_CHECK_GL_ERROR;

		glBindVertexArray(0); TONIC_CHECK_GL_ERROR;
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &m_position_VBO); TONIC_CHECK_GL_ERROR;
		if (m_EBO != 0)
		{
			glDeleteBuffers(1, &m_EBO); TONIC_CHECK_GL_ERROR;
		}
		glDeleteVertexArrays(1, &m_VAO); TONIC_CHECK_GL_ERROR;
	}

	void Mesh::Bind()
	{
		glBindVertexArray(m_VAO); TONIC_CHECK_GL_ERROR;
		glEnableVertexAttribArray(0); TONIC_CHECK_GL_ERROR;
	}

	void Mesh::Unbind()
	{
		glDisableVertexAttribArray(0); TONIC_CHECK_GL_ERROR;
		glBindVertexArray(0); TONIC_CHECK_GL_ERROR;
	} 
};