#include "tonic/core/window.h"
#include "tonic/engine.h"
#include "tonic/log.h"
#include "tonic/app.h"

#include "tonic/graphics/helpers.h"

#include "tonic/input/mouse.h"
#include "tonic/input/keyboard.h"
#include "tonic/input/controller.h"

#include "external/glm/gtc/matrix_transform.hpp"

#include "SDL.h"
#include "glad/glad.h"


namespace tonic::core
{
	//-----------------------------------------

	WindowProperties::WindowProperties()
	{
		title = "TonicApp";
		x_pos = SDL_WINDOWPOS_CENTERED;
		y_pos = SDL_WINDOWPOS_CENTERED;
		width = 1920;
		height = 1080;
		width_min = 320;
		height_min = 180;
		aspect_ratio = 16.f / 9.f;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		clear_color = glm::vec3( 
			0x10 / (float)0xFF, 
			0x1D / (float)0xFF, 
			0x6B / (float)0xFF);
	}

	//-----------------------------------------

	Window::Window()
		: m_render_to_screen(true)
		, m_SDL_window(nullptr)
		, m_GL_context(nullptr)
		, m_frame_buffer(nullptr)
		, m_screen_va(nullptr)
		, m_screen_shader(nullptr)
	{
	}

	//-----------------------------------------

	Window::~Window()
	{
		if (m_SDL_window)
			Shutdown();
	}

	//-----------------------------------------

	bool Window::Create(const WindowProperties& props)
	{
		m_window_properties = props;
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
		glm::vec4 cc = { props.clear_color.r, props.clear_color.g, props.clear_color.b, 1.f };
		m_frame_buffer->SetClearColor(cc);

		Engine::GetInstance().GetRenderManager().SetClearColor(cc);

		InitializeScreenRender();
		HandleResize(props.width, props.height);

		m_imgui_window.Create(props.imgui_props);
		return true;
	}

	//-----------------------------------------

	void Window::Shutdown()
	{
		SDL_DestroyWindow(m_SDL_window);
		SDL_GL_DeleteContext(m_GL_context);
		m_SDL_window = nullptr;
	}

	//-----------------------------------------

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

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
					HandleResize(event.window.data1, event.window.data2);

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

	//-----------------------------------------

	void Window::BeginRender()
	{
		auto& rm = Engine::GetInstance().GetRenderManager();
		rm.Clear();

		rm.Submit(TONIC_SUBMIT_RENDER_CMD(PushFrameBuffer, m_frame_buffer));
	}

	//-----------------------------------------

	void Window::EndRender()
	{
		auto& rm = Engine::GetInstance().GetRenderManager();

		rm.Submit(TONIC_SUBMIT_RENDER_CMD(PopFrameBuffer));
		rm.Flush();

		if (m_render_to_screen)	RenderToScreen();
		

		m_imgui_window.BeginRender();
		Engine::GetInstance().GetApp().ImguiRender();
		m_imgui_window.EndRender();

		SDL_GL_SwapWindow(m_SDL_window);
	}

	//-----------------------------------------

	glm::ivec2 Window::GetSize()
	{
		int w, h;
		SDL_GetWindowSize(m_SDL_window, &w, &h);
		return glm::ivec2(w, h);
	}

	//-----------------------------------------
	//private

	void Window::InitializeScreenRender()
	{
		m_screen_va = std::make_shared<graphics::VertexArray>();

		TONIC_CREATE_VERTEX_BUFFER(vb, short);
		vb->PushVertex({ -1,  1,     0,  1}); // x, y, u, v
		vb->PushVertex({ -1, -1,     0,  0});
		vb->PushVertex({  1, -1,     1,  0});
		vb->PushVertex({  1,  1,     1,  1});
		vb->SetLayout({ 2, 2 });

		m_screen_va->PushBuffer(std::move(vb));
		m_screen_va->SetElements({0,1,2, 0,2,3});
		m_screen_va->Upload();

		const char* screen_vertex_shader = R"(
			#version 410 core
			layout (location = 0) in vec2 pos;
			layout (location = 1) in vec2 tex_coords;
			out vec2 uvs;
			uniform mat4 model = mat4(1.0);
			void main()
			{
				uvs = tex_coords;
				gl_Position = model * vec4(pos, 0.0, 1.0);
			}
		)";

		const char* screen_fragment_shader = R"(
			#version 410 core
			in vec2 uvs;
			out vec4 out_color;
			uniform sampler2D tex;
			void main()
			{
				out_color = texture(tex, uvs);
			}
		)";

		m_screen_shader = std::make_shared<graphics::Shader>(screen_vertex_shader, screen_fragment_shader);
	}

	//----------------------------------------------

	void Window::RenderToScreen()
	{
		TONIC_ASSERT(m_screen_va->IsValid(), "Attempting to render with invalid VertexArray - did you forget t call VertexArray.Upload()?");
		if (!m_screen_va->IsValid())
			return;

		// Black bars
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_screen_va->Bind();
		glBindTexture(GL_TEXTURE_2D, m_frame_buffer->GetTextureID()); TONIC_CHECK_GL_ERROR;
		m_screen_shader->Bind();

		glm::vec2 scale = m_frame_buffer_size / (glm::vec2)GetSize();
		glm::mat4 model(1.0);
		model = glm::scale(model, { scale.x, scale.y, 1.f });
		m_screen_shader->SetUniformMat4("model", model);
		glDrawElements(GL_TRIANGLES, m_screen_va->GetElementCount(), GL_UNSIGNED_INT, 0); TONIC_CHECK_GL_ERROR;

		m_screen_shader->Bind();
		glBindTexture(GL_TEXTURE_2D, 0);
		m_screen_va->Unbind();
	}

	//----------------------------------------------

	void Window::HandleResize(int width, int height)
	{
		int frame_buffer_width  = (int)(height * m_window_properties.aspect_ratio);
		int frame_buffer_height = (int)(width * (1.f / m_window_properties.aspect_ratio));
		
		if (height >= frame_buffer_height)
		{
			frame_buffer_width = width;
		}
		else
		{
			frame_buffer_height = height;
		}
	
		m_frame_buffer_size = { frame_buffer_width, frame_buffer_height };
	}
}