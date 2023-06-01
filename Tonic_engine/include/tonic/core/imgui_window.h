#pragma once

typedef union SDL_Event SDL_Event;
namespace tonic::core
{
	struct ImguiWindowProperties
	{
		bool MoveFromTitleBarOnly = true;
		bool IsDockingEnabled = false;
		bool IsViewportEnabled = false;
	};

	class ImguiWindow
	{
	public:
		ImguiWindow() {}
		~ImguiWindow() {}

		void Create(const ImguiWindowProperties& props);
		void Shutdown();

		void HandleSDLEvent(SDL_Event& event);

		bool WantCaptureMouse();
		bool WantCaptureKeyboard();

		void BeginRender();
		void EndRender();
	private:
	};
}