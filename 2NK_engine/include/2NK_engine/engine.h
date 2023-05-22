#pragma once

#include "core/window.h"

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
		inline void Quit() { is_running = false; }

	private:
		[[nodiscard]] bool Initialize();
		void Shutdown();
		void GetInfo();

	private:
		bool is_running;
		CORE::Window m_window;

		//singleton
		Engine();
		static Engine* m_instance;
	};
}
