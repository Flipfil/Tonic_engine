#pragma once

#include "imgui_window.h"

#include "tonic/graphics/vertex.h"
#include "tonic/graphics/frame_buffer.h"
#include "tonic/graphics/shader.h"

#include "external/glm/glm.hpp"

#include <string>
#include <memory>

struct SDL_Window;
using SDL_GLContext = void*;

namespace tonic::graphics
{
	class FrameBuffer;
}

namespace tonic::core
{
	struct WindowProperties
	{
		std::string title;
		int x_pos, y_pos;
		int width, height;
		int width_min, height_min;
		int flags;
		float aspect_ratio;
		glm::vec3 clear_color;
		ImguiWindowProperties imgui_props;

		WindowProperties();
	};

	class Window
	{
	public:
		Window();
		~Window();

		bool Create(const WindowProperties& props);
		void Shutdown();
		void PumpEvents();

		inline void RenderToScreen(bool enable) { m_render_to_screen = enable; }

		void BeginRender();
		void EndRender();

		inline SDL_Window* GetSDLWindow() { return m_SDL_window; }
		inline SDL_GLContext& GetGLContext() { return m_GL_context; }
		inline graphics::FrameBuffer* GetFrameBuffer() { return m_frame_buffer.get(); }

		glm::ivec2 GetSize();

	private:
		void InitializeScreenRender();
		void RenderToScreen();
		void HandleResize(int width, int height);

	private:
		WindowProperties m_window_properties;

		SDL_Window* m_SDL_window;
		SDL_GLContext m_GL_context;
		ImguiWindow m_imgui_window;

		bool m_render_to_screen;
		glm::vec2 m_frame_buffer_size;
		std::shared_ptr<graphics::FrameBuffer> m_frame_buffer;
		std::shared_ptr<graphics::VertexArray> m_screen_va;
		std::shared_ptr<graphics::Shader>      m_screen_shader;
	};
}