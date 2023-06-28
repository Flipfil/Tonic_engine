#include "tonic/input/mouse.h"
#include "tonic/log.h"

#include "SDL2/SDL_mouse.h"

namespace tonic::input
{
	// statics init
	int Mouse::x = 0;
	int Mouse::x_last = 0;
	int Mouse::y = 0;
	int Mouse::y_last = 0;
	
	std::array<bool, Mouse::m_button_count> Mouse::buttons_down;
	std::array<bool, Mouse::m_button_count> Mouse::buttons_down_last;


	//public
	bool tonic::input::Mouse::HoldingButton(int button)
	{
		TONIC_ASSERT(button >= TONIC_M_BUTTON_FIRST && button <= TONIC_M_BUTTON_LAST, "Invalid mouse button.");
		if (button >= TONIC_M_BUTTON_FIRST && button <= TONIC_M_BUTTON_LAST)
			return buttons_down[button - 1];

		return false;
	}
	bool tonic::input::Mouse::PressedButton(int button)
	{
		TONIC_ASSERT(button >= TONIC_M_BUTTON_FIRST && button <= TONIC_M_BUTTON_LAST, "Invalid mouse button.");
		if (button >= TONIC_M_BUTTON_FIRST && button <= TONIC_M_BUTTON_LAST)
			return buttons_down[button - 1] && !buttons_down_last[button - 1];

		return false;
	}
	bool tonic::input::Mouse::ReleasedButton(int button)
	{
		TONIC_ASSERT(button >= TONIC_M_BUTTON_FIRST && button <= TONIC_M_BUTTON_LAST, "Invalid mouse button.");
		if (button >= TONIC_M_BUTTON_FIRST && button <= TONIC_M_BUTTON_LAST)
			return ! buttons_down[button - 1] && buttons_down_last[button - 1];

		return false;
	}

	void Mouse::Initialize()
	{
		buttons_down.fill(false);
		buttons_down_last.fill(false);
	}

	//private
	void Mouse::Update()
	{
		// position
		x_last = x; y_last = y;
		Uint32 state = SDL_GetMouseState(&x, &y);

		// buttons_down
		for (int i = 0; i < m_button_count; i++)
		{
			buttons_down_last[i] = buttons_down[i];
			buttons_down[i] = state & SDL_BUTTON(i + 1);
		}
	}
}
