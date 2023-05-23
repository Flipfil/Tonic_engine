#include "graphics/mesh.h"

#include "graphics/helpers.h"

#include "glad/glad.h"

namespace TNK::GRAPHICS
{
	Mesh::Mesh(float* vertex_array, uint32_t vertex_count, uint32_t dimensions)
		:m_vertex_count(vertex_count)
		,m_element_count(0)
		,m_EBO(0)
	{
		glGenVertexArrays(1, &m_VAO); TNK_CHECK_GL_ERROR;
		glBindVertexArray(m_VAO); TNK_CHECK_GL_ERROR;

		glGenBuffers(1, &m_position_VBO); TNK_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, m_position_VBO); TNK_CHECK_GL_ERROR;
		glBufferData(GL_ARRAY_BUFFER, m_vertex_count * dimensions * sizeof(float), vertex_array, GL_STATIC_DRAW); TNK_CHECK_GL_ERROR;

		glEnableVertexAttribArray(0); TNK_CHECK_GL_ERROR; 
		glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, 0, 0); TNK_CHECK_GL_ERROR; 
		glDisableVertexAttribArray(0); TNK_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, 0); TNK_CHECK_GL_ERROR;
		

		glBindVertexArray(0); TNK_CHECK_GL_ERROR;
	}

	Mesh::Mesh(float* vertex_array, uint32_t vertex_count, uint32_t dimensions, uint32_t* element_arr, uint32_t element_count)
		:Mesh(vertex_array, vertex_count, dimensions)
	{
		m_element_count = element_count;
		glBindVertexArray(m_VAO); TNK_CHECK_GL_ERROR;

		glGenBuffers(1, &m_EBO); TNK_CHECK_GL_ERROR;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); TNK_CHECK_GL_ERROR;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_element_count * sizeof(float), element_arr, GL_STATIC_DRAW); TNK_CHECK_GL_ERROR;


		glBindVertexArray(0); TNK_CHECK_GL_ERROR;
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &m_position_VBO); // TNK_CHECK_GL_ERROR;
		if (m_EBO != 0)
		{
			glDeleteBuffers(1, &m_EBO); // TNK_CHECK_GL_ERROR;
		}
		glDeleteVertexArrays(1, &m_VAO); // TNK_CHECK_GL_ERROR;
	}

	void Mesh::Bind()
	{
		glBindVertexArray(m_VAO); TNK_CHECK_GL_ERROR;
		glEnableVertexAttribArray(0); TNK_CHECK_GL_ERROR;
	}

	void Mesh::Unbind()
	{
		glDisableVertexAttribArray(0); TNK_CHECK_GL_ERROR;
		glBindVertexArray(0); TNK_CHECK_GL_ERROR;
	} 
};