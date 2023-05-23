#pragma once

#include "graphics/render_commands.h"

#include <queue>

namespace TNK::MANAGERS
{
	class RenderManager
	{
	public:
		RenderManager() {}
		~RenderManager() {}

		void Initialize();
		void Shutdown();

		void SetClearColor(float r, float g, float b, float alpha);

		void Submit(std::unique_ptr<GRAPHICS::RENDER_COMMANDS::RenderCommand> rc);

		//Exexute subitted RenderCoins in the order they were recieved.
		void Flush();

		void Clear();

		void SetWireframeMode(bool enabled);

	private:
		std::queue<std::unique_ptr<GRAPHICS::RENDER_COMMANDS::RenderCommand>> m_render_commands;
	};
}