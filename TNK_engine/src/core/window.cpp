#include "core/window.h"
#include "SDL.h"
#include "engine.h"
#include "log.h"

namespace TNK::CORE
{
	Window::Window() : m_window(nullptr) {}
	Window::~Window()
	{
		if (m_window)
			Shutdown();
	}

	bool Window::Create()
	{
		m_window = SDL_CreateWindow("TNKGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
		if (!m_window)
		{
			TNK_ERROR("Error creating window: ", SDL_GetError());
			return false;
		}
		return true;
	}

	void Window::Shutdown()
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	void Window::PumpEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Engine::GetInstance().Quit();
				break;
			default:
				break;
			}
		}
	}
}