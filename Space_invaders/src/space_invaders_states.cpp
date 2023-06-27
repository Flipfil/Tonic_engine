#include "space_invaders.h"

#include <fstream>

//--------------------------------

void SpaceInvaders::SaveScore()
{
	TONIC_TRACE("SaveScore");
}

//--------------------------------

void SpaceInvaders::NewGame(int players_count)
{
	TONIC_TRACE("New Game");
	m_state = State::GameLoop;
	m_level = 1;

	// create invaders
	m_invaders = CreateInvaders();
	m_invader_speed = INVADER_SPEED;

	// create mothership
	m_mothership = CreateMothership();

	// create hidout
	m_hideout = CreateHideout();

	// create players
	m_players.clear();
	float offset = 2.f / (1 + players_count);
	std::vector<std::string>      colors = { PLAYER_1_COLOR, PLAYER_2_COLOR };
	std::vector<Player::Controls> controls = { PLAYER_1_CONTROLS, PLAYER_2_CONTROLS };
	for (int i = 0; i < players_count; i++)
		m_players.push_back(CreatePlayer(colors[i], controls[i], -1 + (i + 1) * offset));

	m_sound_lib["start_level"]->Play();
	Engine::GetInstance().Sleep(1.f);
}

//--------------------------------

void SpaceInvaders::NextLevel()
{
	TONIC_TRACE("Next Level");

	// increment level counter
	m_level++;
	if (m_level > 5) // its pretty much impossible to win after lvl 5
		m_level = 5;

	// reset player positions and give 1-up
	float offset = 2.f / (1 + m_players.size());
	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->ship->SetPos({ -1 + ((i + 1) * offset), m_players[i]->ship->GetPos().y });
		m_players[i]->ship->SetState(Gameobject::State::Active);

		if (m_players[i]->lives < MAX_LIVES)
		{
			m_players[i]->lives++;
			m_players[i]->lives_indicator[0]->SetTexture(m_texture_lib[std::to_string(m_players[i]->lives)]);
			m_players[i]->lives_indicator[m_players[i]->lives - 1]->SetState(Gameobject::State::Active);
			m_players[i]->shot->SetState(Gameobject::State::Deactivated);
		}
	}

	// reset invaders
	m_invaders = CreateInvaders();
	for (auto& i : m_invaders)
		i->Move({ 0,-0.1f * m_level });
	
	// erase temporary objects
	m_explosions.clear();
	m_invader_shots.clear();

	m_sound_lib["start_level"]->Play();
	Engine::GetInstance().Sleep(1.f);

}

//--------------------------------

void SpaceInvaders::MainMenu()
{
	TONIC_TRACE("Main Menu");
	m_state = State::MainMenu;

	m_title = {
		std::make_shared<Gameobject>(
		m_va_lib["textured_rect"],
		m_material_lib["sprite"],
		m_texture_lib["main_title"],
		TITLE_SIZE,
		TITLE_POS)
	};

	m_selected_option = 0;
	m_options.clear();
	m_options.push_back(CreateText("1 Player",  "white", { 0,        0 }, MENU_LETTER_SIZE));
	m_options.push_back(CreateText("2 Players", "white", { 0, -1 / 4.f }, MENU_LETTER_SIZE));
	m_options.push_back(CreateText("Quit",      "white", { 0, -3 / 4.f }, MENU_LETTER_SIZE));
}

//--------------------------------

void SpaceInvaders::PauseMenu()
{
	TONIC_TRACE("Pause Menu");
	m_state = State::PauseMenu;
	m_title = CreateText("OPTIONS", "white", TITLE_POS, MENU_LETTER_SIZE * 1.5f);

	m_selected_option = 0;
	m_options.clear();
	m_options.push_back(CreateText( "Continue", "white", { 0,        0 }, MENU_LETTER_SIZE));
	m_options.push_back(CreateText("Main Menu", "white", { 0, -1 / 4.f }, MENU_LETTER_SIZE));
	m_options.push_back(CreateText(     "Quit", "white", { 0, -3 / 4.f }, MENU_LETTER_SIZE));
}

//--------------------------------

void SpaceInvaders::GameOver()
{
	TONIC_TRACE("GameOver");
	m_state = State::GameOver;
	m_sound_lib["game_over"]->Play();
	m_selected_option = 0;
	m_options.clear();

	int score = 0;
	for (auto& player : m_players)
		score += player->score;

	auto ScoreToString = [](int score)
	{
		std::string str;
		do {
			int digit = score % 10;
			str.insert(0, 1, '0' + digit);
			score /= 10;
		} while (score > 0);
		return str;
	};

	// save score to file
	if (score > m_high_score)
	{
		std::ofstream os("post_build_copy/high_score");
		if (os.is_open()) os << score;
		else TONIC_ERROR("Can't open high score file");
	}

	if (score > m_high_score)
	{
		m_title = CreateText("New high score! " + ScoreToString(score), "yellow", TITLE_POS, { MENU_LETTER_SIZE.x * 0.4f, MENU_LETTER_SIZE.y});
		m_options.push_back(CreateText("Previous high score: " + ScoreToString(m_high_score), "magenta", { 0, 0 }, { MENU_LETTER_SIZE.x * 0.4f, MENU_LETTER_SIZE.y * 0.8f }));
		m_high_score = score;
	}
	else
	{
		m_title = CreateText("Game Over: " + ScoreToString(score), "red", TITLE_POS, { MENU_LETTER_SIZE.x * 0.4f, MENU_LETTER_SIZE.y});
		m_options.push_back(CreateText("High score: " + ScoreToString(m_high_score), "white", { 0, 0 }, { MENU_LETTER_SIZE.x * 0.4f, MENU_LETTER_SIZE.y * 0.8f }));
	}

	m_options.push_back(CreateText("Play Again", "white", { 0, -1/2.f }, MENU_LETTER_SIZE*0.8f));
	m_options.push_back(CreateText("Main Menu", "white",  { 0, -3/4.f }, MENU_LETTER_SIZE*0.8f));
}

bool SpaceInvaders::IsColliding(const glm::vec2& pos_a, const glm::vec2& size_a, const glm::vec2& pos_b, const glm::vec2& size_b)
{
	float left_a = pos_a.x - size_a.x / 2;
	float right_a = pos_a.x + size_a.x / 2;
	float top_a = pos_a.y + size_a.y / 2;
	float bottom_a = pos_a.y - size_a.y / 2;

	float left_b = pos_b.x - size_b.x / 2;
	float right_b = pos_b.x + size_b.x / 2;
	float top_b = pos_b.y + size_b.y / 2;
	float bottom_b = pos_b.y - size_b.y / 2;

	return (
		left_a < right_b && right_a > left_b &&
		top_a > bottom_b && bottom_a < top_b
		);
}