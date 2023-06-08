#pragma once

#include <array>

namespace tonic::input
{
	class Mouse
	{
	public:
		static void Initialize();
		static void Update();

		inline static int X() { return x; }
		inline static int Y() { return y; }

		inline static int XDelta() { return x - x_last; }
		inline static int YDelta() { return y - y_last; }
		
		static bool HoldingButton(int button);
		static bool PressedButton(int button);
		static bool ReleasedButton(int button);

	public:
		


	private:

		constexpr static const int m_button_count = 5; // SDL supports 5 mouse buttons

		static int x, x_last;
		static int y, y_last;

		static std::array<bool, m_button_count> buttons_down;
		static std::array<bool, m_button_count> buttons_down_last;

	};
}

enum
{
	TONIC_M_BUTTON_FIRST = 1,
	TONIC_M_BUTTON_LEFT = TONIC_M_BUTTON_FIRST,
	TONIC_M_BUTTON_MIDDLE = 2,
	TONIC_M_BUTTON_RIGHT = 3,
	TONIC_M_BUTTON_X1 = 4,
	TONIC_M_BUTTON_X2 = 5,
	TONIC_M_BUTTON_LAST = 6
};
