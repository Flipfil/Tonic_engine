#pragma once

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
		inline void GetSize(uint32_t& w, uint32_t& h) const { w = m_width; h = m_height; }
		inline void SetClearColor(float r, float g, float b, float a) { m_cc_r = r; m_cc_g = g; m_cc_b = b; m_cc_a = a; }
		inline void GetClearColor(float& r, float& g, float& b, float& a) const { r = m_cc_r; g = m_cc_g; b = m_cc_b; a = m_cc_a;}

	private:
		uint32_t m_FBO;
		uint32_t m_texture_ID;
		uint32_t m_render_buffer_ID;

		uint32_t m_width, m_height;
		float m_cc_r, m_cc_g, m_cc_b, m_cc_a;
	};
}