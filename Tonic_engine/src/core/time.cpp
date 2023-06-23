#include "tonic/core/time.h"

#include "tonic/log.h"

#include "SDL.h"

namespace tonic::core
{
	uint32_t Time::m_last = 0;
	float Time::m_total   = 0;
	float Time::m_elapsed = 0;

	void Time::Initialize()
	{
		m_last = SDL_GetTicks();
	}

	float Time::Sleep(float time)
	{
		float time_to_reach = m_total + time;
		float start_t = m_total;
		while (m_total < time_to_reach)
			Update();

		return m_total - start_t;
	}

	void Time::Update()
	{
		uint32_t now = SDL_GetTicks();
		m_elapsed = (now - m_last) / 1000.f;
		m_last = now;
		m_total += m_elapsed;
	}
}