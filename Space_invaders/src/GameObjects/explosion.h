#pragma once

#include "game_object.h"


class Explosion : public AnimatedObject
{
public:
	using AnimatedObject::AnimatedObject;
	void Update() override
	{
		if (GetState() == State::Deactivated)
			return;

		m_delay -= tonic::core::Time::GetElapsed();
		if (m_delay > 0)
			return;

		if (m_frame == m_animation.size() - 1)
			SetState(State::Deactivated);
		

		m_delay = m_frame_rate;
		NextFrame();
		SetTexture(m_animation[m_frame]);
	}
};