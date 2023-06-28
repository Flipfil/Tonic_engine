#include "space_invaders.h"

// These functions in tandem with Player strucure should be a class.
// They require usage of the asset_libs and I really did not want
//  to pass a sub_lib or stuff like that so i just made Player a struct.

// Adds score to given player and updates the score_indicator.
void SpaceInvaders::GainScore(Player& player, int amount)
{
	player.score += amount;
	int score_to_display = player.score;
	if (score_to_display > pow(10, SCORE_DIGITS))
		score_to_display = (int)pow(10, SCORE_DIGITS) - 1;

	int i = SCORE_DIGITS - 1;
	do {
		int digit = score_to_display % 10;
		player.score_indicator[i--]->SetTexture(m_texture_lib[std::string(1, '0' + digit)]);
		score_to_display /= 10;
	} while (score_to_display > 0);
}

// Loses life of given player and updates the score_indicator,
//	plays a special animation that locks everything else in game
void SpaceInvaders::LoseLife(Player& player)
{
	TONIC_TRACE("LoseLife");
	m_sound_lib["life_lost"]->Play();

	// dying animation
	std::weak_ptr<graphics::Material> old_mat = player.ship->GetMaterial();

	player.ship->SetMaterial(m_material_lib["red_silhoulette"]);
	player.ship->SetTexture(m_texture_lib["explosionf0"]);

	Engine::GetInstance().GetWindow().BeginRender();
	Render();
	Engine::GetInstance().GetWindow().EndRender();
	Engine::GetInstance().Sleep(0.5f);

	player.ship->SetTexture(m_texture_lib["explosionf1"]);

	Engine::GetInstance().GetWindow().BeginRender();
	Render();
	Engine::GetInstance().GetWindow().EndRender();
	Engine::GetInstance().Sleep(0.5f);

	player.ship->SetMaterial(old_mat);
	player.ship->SetTexture(m_texture_lib["ship"]);

	// lives update
	player.lives--;
	player.lives_indicator[0]->SetTexture(m_texture_lib[std::to_string(player.lives)]);
	if (player.lives <= 0)
	{
		player.ship->SetState(Gameobject::State::Deactivated);
		return;
	}
	player.lives_indicator[player.lives]->SetState(Gameobject::State::Deactivated);
}