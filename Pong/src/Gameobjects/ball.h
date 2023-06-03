#pragma once

#include "game_object.h"

class Ball : public Gameobject
{
public:
	Ball(std::shared_ptr<tonic::graphics::Mesh> mesh, std::shared_ptr<tonic::graphics::Shader> shader, const glm::vec2& pos, const glm::vec2& size)
		: Gameobject(mesh, shader, pos, size)
		, m_vel(0.f)
	{}

	void SetVelocity(const glm::vec2& vel) { m_vel = vel; }
	const glm::vec2 GetVelocity() { return m_vel; }
	void FlipVelX() { m_vel.x *= -1; }
	void FlipVelY() { m_vel.y *= -1; }

private:
	glm::vec2 m_vel;
};