#pragma once

#include "core/window.h"
#include "managers/logmanager.h"

namespace TNK
{
	class Engine
	{
	public:
		Engine(Engine& other) = delete;
		void operator = (Engine& other) = delete;


		// provide singleton reference
		static Engine& GetInstance();

		void Run();
		inline void Quit() { m_is_running = false; }

	private:
		[[nodiscard]] bool Initialize();
		void Shutdown();
		void GetInfo();

	private:
		bool m_is_running;
		bool m_is_initialized;
		CORE::Window m_window;

		//managers
		MANAGERS::LogManager m_log_manager;

		//singleton
		Engine();
		static Engine* m_instance;
	};
}
