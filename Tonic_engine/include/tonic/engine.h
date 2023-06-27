#pragma once

#include "core/window.h"
#include "managers/logmanager.h"
#include "managers/render_manager.h"

namespace tonic
{
	class App;
	class Engine
	{
	public:
		Engine(Engine& other) = delete;
		void operator = (Engine& other) = delete;


		// provide singleton reference
		static Engine& GetInstance();

		void Run(App* app);
		float Sleep(float time);
		inline void Quit() { m_is_running = false; }

		inline App& GetApp() { return *m_app; }

		inline core::Window& GetWindow() { return m_window; }
		inline managers::RenderManager& GetRenderManager() { return m_render_manager; }

	private:
		[[nodiscard]] bool Initialize();
		void Shutdown();
		void GetInfo();
		void Update();
		void Render();

	private:
		bool m_is_running;
		bool m_is_initialized;
		core::Window m_window;
		App* m_app;

		//managers
		managers::LogManager m_log_manager;
		managers::RenderManager m_render_manager;

		//singleton
		Engine();
		static Engine* m_instance;
	};
}
