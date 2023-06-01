#include "tonic/input/keyboard.h"
#include "tonic/log.h"

#include "SDL_keyboard.h"

namespace tonic::input
{
	std::array<bool, Keyboard::m_key_count> Keyboard::keys_down;
	std::array<bool, Keyboard::m_key_count> Keyboard::keys_down_last;

	//public
	bool tonic::input::Keyboard::KeyHeld(int key)
	{
		TONIC_ASSERT(key >= TONIC_KEY_FIRST && key <= TONIC_KEY_LAST, "Invalid key.");
		if (key >= TONIC_KEY_FIRST && key <= TONIC_KEY_LAST)
			return keys_down[key];

		return false;
	}
	bool tonic::input::Keyboard::KeyPressed(int key)
	{
		TONIC_ASSERT(key >= TONIC_KEY_FIRST && key <= TONIC_KEY_LAST, "Invalid key.");
		if (key >= TONIC_KEY_FIRST && key <= TONIC_KEY_LAST)
			return keys_down[key] && !keys_down_last[key];

		return false;
	}
	bool tonic::input::Keyboard::KeyReleased(int key)
	{
		TONIC_ASSERT(key >= TONIC_KEY_FIRST && key <= TONIC_KEY_LAST, "Invalid key.");
		if (key >= TONIC_KEY_FIRST && key <= TONIC_KEY_LAST)
			return !keys_down[key] && keys_down_last[key];

		return false;
	}

	void Keyboard::Initialize()
	{
		keys_down.fill(false);
		keys_down_last.fill(false);
	}

	//private
	void Keyboard::Update()
	{
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		for (int i = TONIC_KEY_FIRST; i < m_key_count; i++)
		{
			keys_down_last[i] = keys_down[i];
			keys_down[i] = state[i];
		}
	}
}
