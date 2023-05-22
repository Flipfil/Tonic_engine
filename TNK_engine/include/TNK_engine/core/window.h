#pragma once

struct SDL_Window;
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

	private:
		SDL_Window* m_window;
	};
}