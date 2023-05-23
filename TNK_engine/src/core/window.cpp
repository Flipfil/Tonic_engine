#include "core/window.h"
#include "engine.h"
#include "log.h"

#include "SDL.h"
#include "glad/glad.h"

namespace TNK::CORE
{
	Window::Window() : m_SDL_window(nullptr) {}
	Window::~Window()
	{
		if (m_SDL_window)
			Shutdown();
	}

	bool Window::Create()
	{
		m_SDL_window = SDL_CreateWindow("TNKGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!m_SDL_window)
		{
			TNK_ERROR("Error creating window: ", SDL_GetError());
			return false;
		}

#ifdef TNK_PLATFORM_MAC
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_SetWindowMinimumSize(m_SDL_window, 200, 200);

		m_GL_context = SDL_GL_CreateContext(m_SDL_window);
		if (m_GL_context == nullptr)
		{
			TNK_ERROR("Error creating OpenGL context: {}", SDL_GetError());
			return false;
		}

		gladLoadGLLoader(SDL_GL_GetProcAddress);

		return true;
	}

	void Window::Shutdown()
	{
		SDL_DestroyWindow(m_SDL_window);
		m_SDL_window = nullptr;
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

	void Window::BeginRender()
	{
		Engine::GetInstance().GetRenderManager().Clear();
	}

	void Window::EndRender()
	{
		SDL_GL_SwapWindow(m_SDL_window);
	}
}