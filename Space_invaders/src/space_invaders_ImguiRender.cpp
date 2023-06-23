#include "space_invaders.h"

#include "external/imgui/imgui.h"

#ifdef TONIC_CONFIG_DEBUG
void SpaceInvaders::ImguiRender()
{
	if (ImGui::Begin("Invaders"))
	{
		auto& invader = m_invaders[0];
		glm::vec2 pos = invader->GetPos();
		glm::vec2 pos_cur = pos;
		glm::vec2 size = invader->GetSize();
		glm::vec2 size_cur = size;

		ImGui::DragFloat2("top left position", glm::value_ptr(pos), 0.01f);
		if (pos != pos_cur)
		{
			glm::vec2 diff = pos - pos_cur;
			for (auto& i : m_invaders)
				i->Move(diff);
		}

		ImGui::DragFloat2("invader size", glm::value_ptr(size), 0.01f);
		if (size != size_cur)
		{
			for (auto& i : m_invaders)
				i->SetSize(size);
		}
		if (ImGui::Button("On/Off 2 lower rows"))
		{
			bool activate = m_invaders[2*11]->GetState() == Gameobject::State::Deactivated;
			for (int y = 2; y < 5; y++)
				for (int x = 0; x < 11; x++)
					m_invaders[x + y * 11]->SetState(activate ? Gameobject::State::Active : Gameobject::State::Deactivated);
		}
		if (ImGui::Button("On/Off all except 1"))
		{
			bool activate = m_invaders[1]->GetState() == Gameobject::State::Deactivated;
			for (auto& i : m_invaders)
					i->SetState(activate ? Gameobject::State::Active : Gameobject::State::Deactivated);
			m_invaders[0]->SetState(Gameobject::State::Active);
		}
		if (ImGui::Button("Set explosion animation"))
		{
			invader->SetAnimation({ m_texture_lib.Get("explosionf0"), m_texture_lib.Get("explosionf1") }); 
			invader->ResetAnimation();
		}
		if (ImGui::Button("fuc go back")) invader->SetAnimation({ m_texture_lib.Get("invader_0f0"), m_texture_lib.Get("invader_0f1") });

	}
	ImGui::End();

	if (ImGui::Begin("Player 1.ship"))
	{
		auto& player_1 = m_players[0].ship;

		glm::vec2 pos = player_1->GetPos();
		glm::vec2 size = player_1->GetSize();

		ImGui::DragFloat2("position", glm::value_ptr(pos), 0.01f);
		ImGui::DragFloat2("size", glm::value_ptr(size), 0.01f);
		
		player_1->SetPos(pos);
		player_1->SetSize(size);

		if (ImGui::Button("Set red silhoulette"))  player_1->SetMaterial(m_material_lib.Get("red_silhoulette"));
		if (ImGui::Button("Set texture material")) player_1->SetMaterial(m_material_lib.Get("sprite"));
		if (ImGui::Button("LoseLife")) LoseLife(m_players[0]);

	}
	ImGui::End();
}

#else 

void SpaceInvaders::ImguiRender() {}

#endif TONIC_CONFIG_DEBUG
