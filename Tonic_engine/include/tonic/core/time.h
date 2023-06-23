#pragma once

#include <cstdint>

namespace tonic::core
{
	class Time
	{
	public:

		static void Initialize();

		static float Sleep(float time);
		static void  Update();

		static float GetElapsed() { return m_elapsed; };
		static float GetTotal()   { return m_total; };

	private:

		static uint32_t m_last;

		static float m_elapsed;
		static float m_total;
	};
}