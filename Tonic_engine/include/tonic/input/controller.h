#pragma once

#include <array>
#include <memory>
#include <unordered_map>

struct SDL_ControllerDeviceEvent;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;

namespace tonic::input
{
	class Controller
	{
	public:

		enum class Axis
		{								 // Dualshock ref
			LEFT_X, LEFT_Y,		         // left analog stick
			RIGHT_X, RIGHT_Y,			 // right analog stick
			LEFT_TRIGGER, RIGHT_TRIGGER, // 'L2'/'R2' - Dualshock
			COUNT
		};

		enum class Button
		{						   // Dualshock ref
			A, B, X, Y,            // 'cross', 'circle', 'square', 'triangle'
			BACK, GUIDE, START,	   // 'SELECT', 'PSLogo', 'START'
			LS, RS,			       // 'L3/R3' - stick clicks 
			LB, RB,			       // 'L1'/'R1'
			UP, DOWN, LEFT, RIGHT, // arrows
			COUNT
		};

		static void OnControllerConnected(SDL_ControllerDeviceEvent& event);
		static void OnControllerDisconnected(SDL_ControllerDeviceEvent& event);
		static void Update();
		static void Shutdown();

		static bool IsControllerAvailable(int controller_id);

		static bool ButtonHeld(int controller_id, Button button);
		static bool ButtonPressed(int controller_id, Button button);
		static bool ButtonReleased(int controller_id, Button button);
		
		static float GetRawAxis(int controller_id, Axis axis);
		static float GetAxis(int controller_id, Axis axis);
		static float GetAxisDelta(int controller_id, Axis axis);

		static void SetDeadzone(float deadzone);

	private:
		
		struct ControllerStruct
		{
			int controller_id = -1;
			SDL_GameController* gc = nullptr;
			std::array<float, (int)Axis::COUNT> axes;
			std::array<float, (int)Axis::COUNT> axes_last;
			std::array<bool, (int)Button::COUNT> buttons_down;
			std::array<bool, (int)Button::COUNT> buttons_down_last;
		};

		static int GetFreeMapID();

		static std::unordered_map<int, std::unique_ptr<ControllerStruct>> available_controllers;
		static float deadzone;
	};


}