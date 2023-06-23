#include "tonic/graphics/vertex.h"

#include "tonic/graphics/helpers.h"

#include <fstream>

#include "glad/glad.h"

namespace tonic::graphics
{
	const uint32_t RawVertexBuffer::GL_TYPE_BYTE   = GL_BYTE; 
	const uint32_t RawVertexBuffer::GL_TYPE_UBYTE  = GL_UNSIGNED_BYTE;
	const uint32_t RawVertexBuffer::GL_TYPE_SHORT  = GL_SHORT;
	const uint32_t RawVertexBuffer::GL_TYPE_USHORT = GL_UNSIGNED_SHORT;
	const uint32_t RawVertexBuffer::GL_TYPE_INT    = GL_INT;
	const uint32_t RawVertexBuffer::GL_TYPE_UINT   = GL_UNSIGNED_INT;
	const uint32_t RawVertexBuffer::GL_TYPE_FLOAT  = GL_FLOAT;
	const uint32_t RawVertexBuffer::GL_TYPE_DOUBLE = GL_DOUBLE;

	RawVertexBuffer::RawVertexBuffer()
	{
		glGenBuffers(1, &m_VBO); TONIC_CHECK_GL_ERROR;
	}

	RawVertexBuffer::~RawVertexBuffer()
	{
		glDeleteBuffers(1, &m_VBO); TONIC_CHECK_GL_ERROR;
	}

	void RawVertexBuffer::SetLayout(const std::vector<uint32_t>& layout)
	{
		m_layout = layout;
		m_stride = 0;
		for (auto& count : layout)
		{
			m_stride += count;
		}
	}

	void RawVertexBuffer::Upload(bool dynamic)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO); TONIC_CHECK_GL_ERROR;
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); TONIC_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, 0); TONIC_CHECK_GL_ERROR;
		m_is_uploaded = true;
	}

	void RawVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO); TONIC_CHECK_GL_ERROR;
	}

	void RawVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0); TONIC_CHECK_GL_ERROR;
	}

	VertexArray::VertexArray()
		: m_is_valid(false)
		, m_VAO(0)
		, m_EBO(0)
		, m_attribute_count(0)
		, m_vertex_count(0)
		, m_element_count(0)
	{
		glGenVertexArrays(1, &m_VAO); TONIC_CHECK_GL_ERROR;
	}

	VertexArray::~VertexArray()
	{
		m_VBOs.clear();
		glDeleteVertexArrays(1, &m_VAO); TONIC_CHECK_GL_ERROR;
	}

	void VertexArray::PushBuffer(std::unique_ptr<RawVertexBuffer> vbo)
	{
		if (!m_VBOs.empty()) TONIC_ASSERT(m_VBOs[0]->GetVertexCount() == vbo->GetVertexCount(), "VertexArray::PushBuffer - Attempting to push a VertexBuffer with different vertex count.");

		TONIC_ASSERT(! vbo->GetLayout().empty(), "VertexArray::PushBuffer - VertexBuffer has no layout defined");

		if (!vbo->GetLayout().empty())
		{
			m_VBOs.push_back(std::move(vbo));
			m_vertex_count = (uint32_t)m_VBOs[0]->GetVertexCount();
		}
	}

	void VertexArray::SetElements(const std::vector<uint32_t>& elements)
	{
		m_element_count = (uint32_t)elements.size();
		glBindVertexArray(m_VAO); TONIC_CHECK_GL_ERROR;

		glGenBuffers(1, &m_EBO); TONIC_CHECK_GL_ERROR;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); TONIC_CHECK_GL_ERROR;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(uint32_t), &elements[0], GL_STATIC_DRAW);

		glBindVertexArray(0); TONIC_CHECK_GL_ERROR;
	}

	void VertexArray::Upload()
	{
		glBindVertexArray(m_VAO); TONIC_CHECK_GL_ERROR;

		uint32_t attribute_count = 0;
		for (auto& vbo : m_VBOs)
		{
			if (!vbo->IsUploaded())
			{
				vbo->Upload(false);
			}
			vbo->Bind();
			uint32_t offset = 0;
			for (uint32_t count : vbo->GetLayout())
			{
				glEnableVertexAttribArray(attribute_count); TONIC_CHECK_GL_ERROR;
				glVertexAttribPointer(attribute_count, count, static_cast<GLenum>(vbo->GetGLType()), GL_FALSE, vbo->GetStride(), (void*)(intptr_t)offset);

				attribute_count++;
				offset += (count * vbo->GetTypeSize());
			}
			vbo->Unbind();
		}
		
		glBindVertexArray(0); TONIC_CHECK_GL_ERROR;
		m_is_valid = true;
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_VAO); TONIC_CHECK_GL_ERROR;
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0); TONIC_CHECK_GL_ERROR;
	}
}