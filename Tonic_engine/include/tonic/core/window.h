#pragma once

#include "imgui_window.h"

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
		float clear_color_R, clear_color_G, clear_color_B;
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

		void BeginRender();
		void EndRender();

		inline SDL_Window* GetSDLWindow() { return m_SDL_window; }
		inline SDL_GLContext& GetGLContext() { return m_GL_context; }
		inline graphics::FrameBuffer* GetFrameBuffer() { return m_frame_buffer.get(); }

		void GetSize(int& w, int& h);

	private:
		SDL_Window* m_SDL_window;
		SDL_GLContext m_GL_context;
		ImguiWindow m_imgui_window;
		std::shared_ptr<graphics::FrameBuffer> m_frame_buffer;
	};
}