#pragma once

#include "game_object.h"

#include "tonic/core/time.h"

// animated object with set trajectory
class Projectile : public AnimatedObject
{
public:
	Projectile(
		std::weak_ptr<tonic::graphics::VertexArray> va,
		std::weak_ptr<tonic::graphics::Material> material,
		std::vector<std::weak_ptr<tonic::graphics::Texture>> animation,
		const glm::vec2& size = glm::vec2(0.5f),
		const glm::vec2& velocity = glm::vec2(0.f),
		float fps = 0,
		const glm::vec2& pos = glm::vec2(0.f),
		State state = State::Active)
		:
		AnimatedObject(va, material, animation, size, fps, pos, state)
		, m_velocity(velocity)
	{}

	void Update() override
	{
		if (GetState() == Gameobject::State::Deactivated)
			return;

		Move(m_velocity * tonic::core::Time::GetElapsed());
		AnimatedObject::Update();
	}

private:
	glm::vec2 m_velocity;
};