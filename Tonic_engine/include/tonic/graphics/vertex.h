#pragma once

#include "tonic/log.h"

#include <cstdint>
#include <vector>
#include <type_traits>

#define TONIC_CREATE_VERTEX_BUFFER(name, type) std::unique_ptr<tonic::graphics::VertexBuffer<type>> name = std::make_unique<tonic::graphics::VertexBuffer<type>>()

namespace tonic::graphics
{
	class RawVertexBuffer
	{
	public:
		static const uint32_t GL_TYPE_BYTE;
		static const uint32_t GL_TYPE_UBYTE;
		static const uint32_t GL_TYPE_SHORT;
		static const uint32_t GL_TYPE_USHORT;
		static const uint32_t GL_TYPE_INT;
		static const uint32_t GL_TYPE_UINT;
		static const uint32_t GL_TYPE_FLOAT;
		static const uint32_t GL_TYPE_DOUBLE;

	public:
		RawVertexBuffer();
		virtual ~RawVertexBuffer();

		virtual uint32_t GetTypeSize() const = 0;

		inline       bool		            IsUploaded()     const { return m_is_uploaded; }
		inline       uint32_t               GetID()          const { return m_VBO; }
		inline       uint32_t               GetVertexCount() const { return m_vertex_count; }
		inline       uint32_t               GetStride()      const { return m_stride; }
		inline		 uint32_t				GetGLType()      const { return m_GL_type; }
		inline const std::vector<uint32_t>& GetLayout()      const { return m_layout; }

		virtual void Upload(bool dynamic = false);

		void SetLayout(const std::vector<uint32_t>& layout);
		void Bind();
		void Unbind();


	protected:
		bool	 m_is_uploaded = false;
		uint32_t m_VBO = 0;
		uint32_t m_vertex_count = 0;
		uint32_t m_stride = 0;

		std::vector<uint32_t> m_layout;
		void* m_data = nullptr;
		uint32_t m_size = 0;
		uint32_t m_GL_type = 0;
	};

	template<typename T>
	class VertexBuffer : public RawVertexBuffer
	{
		// restrict template types at compile time
		static_assert(
			std::is_same<T, char          >() || // GL_BYTE
			std::is_same<T, unsigned char >() || // GL_UNSIGNED_BYTE
			std::is_same<T, short         >() || // GL_SHORT
			std::is_same<T, unsigned short>() || // GL_UNSIGNED_SHORT
			std::is_same<T, int           >() || // GL_INT
			std::is_same<T, unsigned int  >() || // GL_UNSIGNED_INT
			std::is_same<T, float         >() || // GL_FLOAT
			std::is_same<T, double        >()    // GL_DOUBLE
			, "This type is not supported.");
	public:
		VertexBuffer() 
			: m_value_count(0)
		{
			// set data type at compile time
			if constexpr (std::is_same<T, char          >()) { m_GL_type = RawVertexBuffer::GL_TYPE_BYTE;   }
			if constexpr (std::is_same<T, unsigned char >()) { m_GL_type = RawVertexBuffer::GL_TYPE_UBYTE;  }
			if constexpr (std::is_same<T, short         >()) { m_GL_type = RawVertexBuffer::GL_TYPE_SHORT;  }
			if constexpr (std::is_same<T, unsigned short>()) { m_GL_type = RawVertexBuffer::GL_TYPE_USHORT; }
			if constexpr (std::is_same<T, int           >()) { m_GL_type = RawVertexBuffer::GL_TYPE_INT;    }
			if constexpr (std::is_same<T, unsigned int  >()) { m_GL_type = RawVertexBuffer::GL_TYPE_UINT;   }
			if constexpr (std::is_same<T, float         >()) { m_GL_type = RawVertexBuffer::GL_TYPE_FLOAT;  }
			if constexpr (std::is_same<T, double        >()) { m_GL_type = RawVertexBuffer::GL_TYPE_DOUBLE; }
		}
		~VertexBuffer() {}

		uint32_t GetTypeSize() const { return sizeof(T); }

		void PushVertex(const std::vector<T>& vertex)
		{
			TONIC_ASSERT(!vertex.empty(), "Passed an empty vertex");

			if (m_data_vec.empty())
				m_value_count = (uint32_t)vertex.size();
			
			TONIC_ASSERT(vertex.size() == m_value_count, "Attempting to push a vertex with unexpected size. Expected size is determined by the first pushed vertex.");
			if (vertex.size() == m_value_count)
			{
				m_vertex_count++;
				m_data_vec.insert(m_data_vec.end(), vertex.begin(), vertex.end());
			}
		}

		void Upload(bool dynamic = false) override
		{
			m_stride *= sizeof(T);
			m_size = sizeof(T) * (uint32_t)m_data_vec.size();
			TONIC_TRACE("VertexBuffer::Upload() - m_size: {}, m_stride: {}", m_size, m_stride);
			TONIC_ASSERT(m_size > 0, "VertexBuffer::Upload() - size = 0 ");
			m_data = &m_data_vec[0]; // vector assures contiguous data
			RawVertexBuffer::Upload(dynamic);
		}

	private:
		uint32_t m_value_count;
		std::vector<T> m_data_vec;
	};
	
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		inline bool IsValid() const { return m_is_valid; }
		inline uint32_t GetVertexCount() const { return m_vertex_count; }
		inline uint32_t GetElementCount() const { return m_element_count; }
		
		void PushBuffer(std::unique_ptr<RawVertexBuffer> vbo);
		void SetElements(const std::vector<uint32_t>& elements);

		void Upload();

		void Bind();
		void Unbind();

	private:
		bool m_is_valid;
		uint32_t m_vertex_count, m_element_count;
		uint32_t m_VAO, m_EBO;
		uint32_t m_attribute_count;
		std::vector<std::unique_ptr<RawVertexBuffer>> m_VBOs;
	};
}