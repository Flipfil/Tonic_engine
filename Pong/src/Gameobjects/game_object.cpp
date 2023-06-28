#include "game_object.h"

#include "tonic/engine.h"
#include "external/glm/gtc/matrix_transform.hpp"

void Gameobject::Update()
{
}

void Gameobject::Render()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, {m_pos.x, m_pos.y, 0.f});
	model = glm::scale(model, { m_size.x, m_size.y, 0.f });
	tonic::Engine::GetInstance().GetRenderManager().Submit(TONIC_SUBMIT_RENDER_CMD(RenderVertexArray, m_VA, m_shader, model));
}