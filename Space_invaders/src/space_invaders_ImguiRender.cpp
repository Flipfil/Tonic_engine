#include "space_invaders.h"

#include "external/imgui/imgui.h"

#ifdef TONIC_CONFIG_DEBUG
void SpaceInvaders::ImguiRender()
{
	if (m_state == State::GameLoop)
	{
		if (ImGui::Begin("Invaders"))
		{
			auto& invader = m_invaders[0];
			glm::vec2 pos = invader->GetPos();
			glm::vec2 pos_cur = pos;
			glm::vec2 size = invader->GetSize();
			glm::vec2 size_cur = size;

			ImGui::DragFloat2("top left invader position:", glm::value_ptr(pos), 0.01f);
			if (pos != pos_cur)
			{
				glm::vec2 diff = pos - pos_cur;
				for (auto& i : m_invaders)
					i->Move(diff);
			}

			ImGui::DragFloat2("invader size:", glm::value_ptr(size), 0.01f);
			if (size != size_cur)
			{
				for (auto& i : m_invaders)
					i->SetSize(size);
			}
			if (ImGui::Button("On/Off 1st"))
			{
				m_invaders[0]->SetState(
					m_invaders[1]->GetState() == Gameobject::State::Deactivated
					? Gameobject::State::Active
					: Gameobject::State::Deactivated);
			}
			if (ImGui::Button("Activate all except 1"))
			{
				for (int i = 1; i < INVADERS_IN_COL * INVADERS_IN_ROW; i++)
					m_invaders[i]->SetState(Gameobject::State::Active);
			}
			if (ImGui::Button("Dectivate all except 1"))
			{
				for (int i = 1; i < INVADERS_IN_COL * INVADERS_IN_ROW; i++)
					m_invaders[i]->SetState(Gameobject::State::Deactivated);
			}
		}
		ImGui::End();

		if (ImGui::Begin("Player 1"))
		{
			auto& player_1 = m_players[0]->ship;

			glm::vec2 pos = player_1->GetPos();
			glm::vec2 size = player_1->GetSize();

			ImGui::DragFloat2("position", glm::value_ptr(pos), 0.01f);
			ImGui::DragFloat2("size", glm::value_ptr(size), 0.01f);
			ImGui::DragFloat("speed", &(m_players[0]->speed), 0.01f);


			player_1->SetPos(pos);
			player_1->SetSize(size);

			if (ImGui::Button("Set red silhoulette"))  player_1->SetMaterial(m_material_lib.Get("red_silhoulette"));
			if (ImGui::Button("Set texture material")) player_1->SetMaterial(m_material_lib.Get("sprite"));
			if (ImGui::Button("LoseLife"))
			{
				ImGui::End();
				ImGui::EndFrame();
				LoseLife(*m_players[0]);
				ImGui::NewFrame();
				return;
			}
			if (ImGui::Button("GainScore 150")) GainScore(*m_players[0], 150);
			if (ImGui::Button("GainScore 999999")) GainScore(*m_players[0], 999999);

		}
		ImGui::End();
	}

	if (ImGui::Begin("Game state"))
	{
		if (ImGui::Button("Next Level")) NextLevel();
		if (ImGui::Button("New Game 1 player"))  NewGame(1);
		if (ImGui::Button("New Game 2 players"))  NewGame(2);
		if (ImGui::Button("Main Menu"))  MainMenu();
		if (ImGui::Button("Pause Menu"))  PauseMenu();
		if (ImGui::Button("Game Over"))  GameOver();
		if (ImGui::Button("Save Score")) GameOver();
	}
	ImGui::End();

	if (ImGui::Begin("Sounds"))
	{
		if (ImGui::Button("Start level")) m_sound_lib["start_level"]->Play();
	}
	ImGui::End();
}

#else 

void SpaceInvaders::ImguiRender() {}

#endif TONIC_CONFIG_DEBUG
