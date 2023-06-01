#pragma once

#include "external/glm/glm.hpp"

#include <cstdint>

namespace tonic::graphics
{
	class FrameBuffer
	{
	public:
		FrameBuffer(uint32_t width, uint32_t height);
		~FrameBuffer();

		inline uint32_t GetFBO() const {return m_FBO;}
		inline uint32_t GetTextureID() const {return m_texture_ID;}
		inline uint32_t GetRenderBufferID() const {return m_render_buffer_ID;}
		inline const glm::ivec2& GetSize() const { return m_size; }
		inline void SetClearColor(const glm::vec4& clear_color) { m_clear_color = clear_color; }
		inline const glm::vec4& GetClearColor() const { return m_clear_color; }

	private:
		uint32_t m_FBO;
		uint32_t m_texture_ID;
		uint32_t m_render_buffer_ID;

		glm::ivec2 m_size;
		glm::vec4 m_clear_color;
	};
}