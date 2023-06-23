#include "tonic/managers/render_manager.h"

#include "tonic/graphics/helpers.h"
#include "tonic/graphics/frame_buffer.h"
#include "tonic/engine.h"

#include "tonic/log.h"

#include "glad/glad.h"

namespace tonic::managers
{
	//--------------------------------

	void RenderManager::Initialize()
	{
		TONIC_INFO("OpenGL Info:\n Vendor:\t{}\n Renderer:\t{}\n Verison:\t{}",
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION));

		// Initialize OpenGL
		glEnable(GL_DEPTH_TEST); TONIC_CHECK_GL_ERROR; // tell OpenGL that it can skip rendering stuff deeper than specified
		glDepthFunc(GL_LEQUAL); TONIC_CHECK_GL_ERROR;// specify depth test func - dont render stuff behind other objects

		glEnable(GL_BLEND); TONIC_CHECK_GL_ERROR; // allows transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); TONIC_CHECK_GL_ERROR;
	}

	//--------------------------------

	void RenderManager::Shutdown()
	{
		while (!m_render_commands.empty())
			m_render_commands.pop();
	}

	//--------------------------------

	void RenderManager::SetClearColor(const glm::vec4 clear_color)
	{
		glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a); TONIC_CHECK_GL_ERROR;
	}

	//--------------------------------

	void RenderManager::Submit(std::unique_ptr<graphics::RENDER_COMMANDS::RenderCommand> rc)
	{
		m_render_commands.push(std::move(rc));
	}

	//--------------------------------

	void RenderManager::Flush()
	{
		while (!m_render_commands.empty())
		{
			auto rc = std::move(m_render_commands.front());
			m_render_commands.pop();

			rc->Execute();
		}
	}

	//--------------------------------

	void RenderManager::SetViewPort(const glm::ivec4 dimensions)
	{
		glViewport(dimensions.x, dimensions.y, dimensions.z, dimensions.w); TONIC_CHECK_GL_ERROR;
	}

	//--------------------------------

	void RenderManager::Clear()
	{
		TONIC_ASSERT(m_render_commands.empty(), "Unflushed render commands in queue.");
		while (!m_render_commands.empty())
			m_render_commands.pop();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TONIC_CHECK_GL_ERROR;
	}

	//--------------------------------

	void RenderManager::SetWireframeMode(bool enabled)
	{
		if (enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); TONIC_CHECK_GL_ERROR;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); TONIC_CHECK_GL_ERROR;
		}
	}

	//--------------------------------

	void RenderManager::PushFrameBuffer(std::shared_ptr<graphics::FrameBuffer> fb)
	{
		TONIC_ASSERT(fb, "FrameBuffer is null.");
		m_frame_buffers.push(fb);
		glBindFramebuffer(GL_FRAMEBUFFER, fb->GetFBO()); TONIC_CHECK_GL_ERROR;
		SetViewPort({ 0, 0, fb->GetSize().x, fb->GetSize().y });

		auto& cc = fb->GetClearColor();
		glClearColor(cc.r, cc.g, cc.b, cc.a); TONIC_CHECK_GL_ERROR;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TONIC_CHECK_GL_ERROR;
	}
	
	//--------------------------------

	void RenderManager::PopFrameBuffer()
	{
		TONIC_ASSERT(!m_frame_buffers.empty(), "RenderManager::PopFrameBuffer - empty stack.");
		if (m_frame_buffers.empty())
			return;

		m_frame_buffers.pop();
		if (!m_frame_buffers.empty())
		{
			auto next_fb = m_frame_buffers.top();
			glBindFramebuffer(GL_FRAMEBUFFER, next_fb->GetFBO()); TONIC_CHECK_GL_ERROR;
			SetViewPort({ 0, 0, next_fb->GetSize().x, next_fb->GetSize().y });
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0); TONIC_CHECK_GL_ERROR;
			auto& window = Engine::GetInstance().GetWindow();
			SetViewPort({ 0, 0, window.GetSize().x, window.GetSize().y });
		}
	}

	//--------------------------------

	void RenderManager::PushCamera(std::shared_ptr<graphics::Camera> camera)
	{
		TONIC_ASSERT(camera, "Camera is null.");
		m_cameras.push(camera);
	}

	//--------------------------------

	void RenderManager::PopCamera()
	{
		TONIC_ASSERT(!m_cameras.empty(), "RenderManager::PopCamera - empty stack.");
		if (m_cameras.empty())
			return;
		
		m_cameras.pop();
	}
}