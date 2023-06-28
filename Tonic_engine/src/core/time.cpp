#include "tonic/core/time.h"

#include "tonic/log.h"

#include "SDL2/SDL.h"

namespace tonic::core
{
	uint32_t Time::m_last = 0;
	float Time::m_total   = 0;
	float Time::m_elapsed = 0;

	void Time::Initialize()
	{
		m_last = SDL_GetTicks();
	}

	void Time::Update()
	{
		uint32_t now = SDL_GetTicks();
		m_elapsed = (now - m_last) / 1000.f;
		m_last = now;
		m_total += m_elapsed;
	}
}