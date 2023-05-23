#pragma once

struct SDL_Window;
using SDL_GLContext = void*;
namespace TNK::CORE
{
	class Window
	{
	public:
		Window();
		~Window();

		bool Create();
		void Shutdown();
		void PumpEvents();

		void BeginRender();
		void EndRender();

	private:
		SDL_Window* m_SDL_window;
		SDL_GLContext m_GL_context;
	};
}