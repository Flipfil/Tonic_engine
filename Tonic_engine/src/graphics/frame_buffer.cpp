#include "tonic/graphics/frame_buffer.h"
#include "tonic/log.h"

#include "tonic/graphics/helpers.h"

#include "glad/glad.h"

namespace tonic::graphics
{
	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) 
		: m_FBO(0)
		, m_texture_ID(0)
		, m_render_buffer_ID(0)
		, m_width(width)
		, m_height(height)
		, m_cc_r(1)
		, m_cc_g(1)
		, m_cc_b(1)
		, m_cc_a(1)
	{
		glGenFramebuffers(1, &m_FBO); TONIC_CHECK_GL_ERROR;
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); TONIC_CHECK_GL_ERROR;

		// Create color texture
		glGenTextures(1, &m_texture_ID); TONIC_CHECK_GL_ERROR;
		glBindTexture(GL_TEXTURE_2D, m_texture_ID); TONIC_CHECK_GL_ERROR;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); TONIC_CHECK_GL_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); TONIC_CHECK_GL_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); TONIC_CHECK_GL_ERROR;
		glBindTexture(GL_TEXTURE_2D, 0); TONIC_CHECK_GL_ERROR;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_ID, 0); TONIC_CHECK_GL_ERROR;

		// Crate depth/stencil renderbuffer
		glGenRenderbuffers(1, &m_render_buffer_ID); TONIC_CHECK_GL_ERROR;
		glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_ID); TONIC_CHECK_GL_ERROR;
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);  TONIC_CHECK_GL_ERROR;
		glBindRenderbuffer(GL_RENDERBUFFER, 0); TONIC_CHECK_GL_ERROR;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_ID); TONIC_CHECK_GL_ERROR;

		// Check for completeness
		int32_t complete_status = glCheckFramebufferStatus(GL_FRAMEBUFFER); TONIC_CHECK_GL_ERROR;
		if (complete_status != GL_FRAMEBUFFER_COMPLETE)
		{
			TONIC_ERROR("Failed to create framebuffer. Complete status: {}", complete_status);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0); TONIC_CHECK_GL_ERROR;
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
		m_FBO = 0;
		m_texture_ID = 0;
		m_render_buffer_ID = 0;
	}
}