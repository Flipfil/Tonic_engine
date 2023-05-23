#include "managers/render_manager.h"

#include "graphics/helpers.h"

#include "tnk_assert.h"

#include "glad/glad.h"

namespace TNK::MANAGERS
{
	void RenderManager::Initialize()
	{
		TNK_INFO("OpenGL Info:\n Vendor:\t{}\n Renderer:\t{}\n Verison:\t{}",
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION));

		// Initialize OpenGL
		glEnable(GL_DEPTH_TEST); TNK_CHECK_GL_ERROR; // tell OpenGL that it can skip rendering stuff deeper than specified
		glDepthFunc(GL_LEQUAL); TNK_CHECK_GL_ERROR;// specify depth test func - dont render stuff behind other objects

		glEnable(GL_BLEND); TNK_CHECK_GL_ERROR; // allows transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); TNK_CHECK_GL_ERROR;

		SetClearColor(0x10 / (float)0xFF,
			0x1D / (float)0xFF,
			0x6B / (float)0xFF,
			1);
	}

	void RenderManager::Shutdown()
	{
		while (!m_render_commands.empty())
			m_render_commands.pop();
	}

	void RenderManager::SetClearColor(float r, float g, float b, float alpha)
	{
		glClearColor(r,g,b,alpha); TNK_CHECK_GL_ERROR;
	}

	void RenderManager::Submit(std::unique_ptr<GRAPHICS::RENDER_COMMANDS::RenderCommand> rc)
	{
		m_render_commands.push(std::move(rc));
	}

	void RenderManager::Flush()
	{
		while (!m_render_commands.empty())
		{
			auto rc = std::move(m_render_commands.front());
			m_render_commands.pop();

			rc->Execute();
		}
	}

	void RenderManager::Clear()
	{
		TNK_ASSERT(m_render_commands.empty(), "Unflushed render commands in queue.");
		while (!m_render_commands.empty())
			m_render_commands.pop();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TNK_CHECK_GL_ERROR;
	}

	void RenderManager::SetWireframeMode(bool enabled)
	{
		if (enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); TNK_CHECK_GL_ERROR;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); TNK_CHECK_GL_ERROR;
		}
	}

}