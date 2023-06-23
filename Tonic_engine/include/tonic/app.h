#pragma once

#include "tonic/core/window.h"

namespace tonic
{
	class App
	{
	public:
		App() {}
		~App() {}

		virtual core::WindowProperties GetWindowProperties() { return core::WindowProperties(); }

		virtual void Initialize() {} // initialize your game
		virtual void Shutdown() {}   // clean up after yourself

		virtual void Update() {}     // update game objects
		virtual void Render() {}     // render game objects

		virtual void ImguiRender() {} // render ImGui

	private:
	};
}