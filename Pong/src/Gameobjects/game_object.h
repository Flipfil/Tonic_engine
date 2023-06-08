#pragma once

#include "tonic/graphics/vertex.h"
#include "tonic/graphics/shader.h"

#include "external/glm/glm.hpp"

#include <memory>

class Gameobject
{
public:
	Gameobject(std::shared_ptr<tonic::graphics::VertexArray> va, std::shared_ptr<tonic::graphics::Shader> shader, const glm::vec2& pos, const glm::vec2& size)
		: m_VA(va)
		, m_shader(shader)
		, m_pos(pos)
		, m_size(size)
	{}

	void SetPos(const glm::vec2& pos) { m_pos = pos; }
	void Move(const glm::vec2& by) { m_pos += by; }

	const glm::vec2& GetPos() { return m_pos; }
	const glm::vec2& GetSize() { return m_size; }

	void Update();
	void Render();

private:
	std::shared_ptr<tonic::graphics::VertexArray> m_VA;
	std::shared_ptr<tonic::graphics::Shader> m_shader;

	glm::vec2 m_pos;
	glm::vec2 m_size;
};