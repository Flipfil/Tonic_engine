#include "tonic/core/window.h"
#include "tonic/engine.h"
#include "tonic/log.h"
#include "tonic/app.h"

#include "tonic/graphics/frame_buffer.h"

#include "tonic/input/mouse.h"
#include "tonic/input/keyboard.h"
#include "tonic/input/controller.h"

#include "SDL.h"
#include "glad/glad.h"


namespace tonic::core
{
	WindowProperties::WindowProperties()
	{
		title = "TonicApp";
		x_pos = SDL_WINDOWPOS_CENTERED;
		y_pos = SDL_WINDOWPOS_CENTERED;
		width = 1920;
		height = 1080;
		width_min = 320;
		height_min = 180;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		clear_color_R = 0x10 / (float)0xFF;
		clear_color_G = 0x1D / (float)0xFF;
		clear_color_B = 0x6B / (float)0xFF;
	}

	Window::Window() : m_SDL_window(nullptr), m_GL_context(nullptr) {}
	Window::~Window()
	{
		if (m_SDL_window)
			Shutdown();
	}

	bool Window::Create(const WindowProperties& props)
	{
		m_SDL_window = SDL_CreateWindow(props.title.c_str(), props.x_pos, props.y_pos, props.width, props.height, props.flags);
		if (!m_SDL_window)
		{
			TONIC_ERROR("Error creating window: ", SDL_GetError());
			return false;
		}

#ifdef TONIC_PLATFORM_MAC
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


		SDL_SetWindowMinimumSize(m_SDL_window, props.width_min, props.height_min);

		m_GL_context = SDL_GL_CreateContext(m_SDL_window);
		if (m_GL_context == nullptr)
		{
			TONIC_ERROR("Error creating OpenGL context: {}", SDL_GetError());
			return false;
		}

		gladLoadGLLoader(SDL_GL_GetProcAddress);

		m_frame_buffer = std::make_shared<graphics::FrameBuffer>(props.width, props.height);
		m_frame_buffer->SetClearColor(props.clear_color_R, props.clear_color_G, props.clear_color_B, 1.f);

		m_imgui_window.Create(props.imgui_props);
		return true;
	}

	void Window::Shutdown()
	{
		SDL_DestroyWindow(m_SDL_window);
		SDL_GL_DeleteContext(m_GL_context);
		m_SDL_window = nullptr;
	}

	void Window::PumpEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			m_imgui_window.HandleSDLEvent(event);
			switch (event.type)
			{
			case SDL_QUIT:
				Engine::GetInstance().Quit();
				break;

			case SDL_CONTROLLERDEVICEADDED:
				input::Controller::OnControllerConnected(event.cdevice);
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				input::Controller::OnControllerDisconnected(event.cdevice);
				break;

			default:
				break;
			}
		}

		// Update input
		if (!m_imgui_window.WantCaptureMouse())
			input::Mouse::Update();
		if (!m_imgui_window.WantCaptureKeyboard())
			input::Keyboard::Update();
		input::Controller::Update();
	}

	void Window::BeginRender()
	{
		auto& rm = Engine::GetInstance().GetRenderManager();
		rm.Clear();
		rm.Submit(TONIC_SUBMIT_RENDER_CMD(PushFrameBuffer, m_frame_buffer));
	}

	void Window::EndRender()
	{
		auto& rm = Engine::GetInstance().GetRenderManager();
		rm.Submit(TONIC_SUBMIT_RENDER_CMD(PopFrameBuffer));
		rm.Flush();

		m_imgui_window.BeginRender();
		Engine::GetInstance().GetApp().ImguiRender();
		m_imgui_window.EndRender();

		SDL_GL_SwapWindow(m_SDL_window);
	}

	void Window::GetSize(int& w, int& h)
	{
		SDL_GetWindowSize(m_SDL_window, &w, &h);
	}
}