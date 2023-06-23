#include "game_object.h"

#include "tonic/engine.h"
#include "external/glm/gtc/matrix_transform.hpp"

#include "external/imgui/imgui.h"

//-----------------------------------

void Gameobject::Update()
{
}

void AnimatedObject::Update()
{
	if (GetState() == State::Deactivated)
		return;

	m_delay -= tonic::core::Time::GetElapsed();
	if (m_delay > 0)
		return;

	m_delay = m_frame_rate;
	NextFrame();
	SetTexture(m_animation[m_frame]);
}

//-----------------------------------

void Gameobject::Render()
{
	if (m_state == State::Deactivated)
		return;

	tonic::Engine::GetInstance().GetRenderManager().Submit(TONIC_SUBMIT_RENDER_CMD(RenderVertexArrayMaterialTextured, m_VA, m_material, m_texture, m_model));
}

//-----------------------------------

void Gameobject::Move(const glm::vec2& by)
{
	if (by == glm::vec2(0.f))
		return;

	m_pos += by;
	CalculateModelMat();
}

//-----------------------------------

void Gameobject::Scale(const glm::vec2& by)
{
	if (by == glm::vec2(0.f))
		return;

	m_size *= by;
	CalculateModelMat();
}

//-----------------------------------

void Gameobject::SetPos(const glm::vec2& pos)
{
	if (pos == m_pos)
		return;

	m_pos = pos;
	CalculateModelMat();
}

//-----------------------------------

void Gameobject::SetSize(const glm::vec2& size)
{
	if (size == m_size)
		return;

	m_size = size;
	CalculateModelMat();
}

//-----------------------------------

void Gameobject::CalculateModelMat()
{
	m_model = glm::mat4(1.f);
	m_model = glm::translate(m_model, { m_pos.x, m_pos.y, 0.f });
	m_model = glm::scale(m_model, { m_size.x, m_size.y, 0.f });
}


