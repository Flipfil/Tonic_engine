#include "space_invaders.h"

//-------------------------------------------------------------
// Text

std::vector<std::shared_ptr<Gameobject>> SpaceInvaders::CreateText(const std::string& str, const std::string& color, const glm::vec2& pos, const glm::vec2& letter_size)
{
	glm::vec2 letter_position = pos;
	letter_position.x -= (str.size() * letter_size.x / 2.f);
	std::vector<std::shared_ptr<Gameobject>> text;
	for (auto c : str)
		text.push_back(std::make_shared<Gameobject>(
			m_va_lib.Get("textured_rect"),
			m_material_lib.Get(color + "_silhoulette"),
			m_texture_lib.Get(std::string(1, c)),
			letter_size,
			letter_position += glm::vec2(letter_size.x, 0)));

	return text;
}



//-------------------------------------------------------------
// Shots

std::shared_ptr<Projectile> SpaceInvaders::CreateBlastShot(const glm::vec2& direction)
{
	return std::make_shared<Projectile>(
		m_va_lib["textured_rect"],
		m_material_lib["red_silhoulette"],
		*m_animation_lib["shot_blast"],
		SHOT_SIZE,
		direction * SHOT_BLAST_SPEED,
		10.f);
}

std::shared_ptr<Projectile> SpaceInvaders::CreateSquigglyShot(const glm::vec2& direction)
{
	return std::make_shared<Projectile>(
		m_va_lib["textured_rect"],
		m_material_lib["yellow_silhoulette"],
		*m_animation_lib["shot_squiggly"],
		SHOT_SIZE,
		direction * SHOT_SQUIGGLY_SPEED,
		10.f);
}

std::shared_ptr<Projectile> SpaceInvaders::CreateSpiralShot(const glm::vec2& direction)
{
	return std::make_shared<Projectile>(
		m_va_lib["textured_rect"],
		m_material_lib["magenta_silhoulette"],
		*m_animation_lib["shot_spiral"],
		SHOT_SIZE,
		direction * SHOT_SPIRAL_SPEED,
		10.f);
}

//-------------------------------------------------------------
// Player

std::shared_ptr<Player> SpaceInvaders::CreatePlayer(const std::string& color, Player::Controls controls, float start_x)
{
	Player player;

	// controls
	player.controls = controls;

	// shot
	player.shot = CreateBlastShot({0,1});
	player.shot->SetMaterial(m_material_lib[color + "_silhoulette"]);
	player.shot->SetState(Gameobject::State::Deactivated);

	// ship
	player.ship = std::make_shared<Gameobject>(
		m_va_lib["textured_rect"],
		m_material_lib[color + "_silhoulette"],
		m_texture_lib["ship"], 
		PLAYER_SIZE,
		glm::vec2(start_x, -1 + BOTTOM_BORDER_HEIGHT + PLAYER_SIZE.y/2));

	player.speed = PLAYER_SPEED;

	// lives
	glm::vec2 pos = {start_x * 2.f - BOTTOM_BORDER_HEIGHT, -1 + BOTTOM_BORDER_HEIGHT / 2.f};
	player.lives = MAX_LIVES;
	player.lives_indicator.push_back(std::make_shared<Gameobject>(
		m_va_lib["textured_rect"],
		m_material_lib[color + "_silhoulette"],
		m_texture_lib[std::to_string(MAX_LIVES)],
		glm::vec2(BOTTOM_BORDER_HEIGHT * 0.8f / 2.f, BOTTOM_BORDER_HEIGHT * 0.8f),
		pos));

	for (int i = 0; i < MAX_LIVES - 1; i++)
		player.lives_indicator.push_back(std::make_shared<Gameobject>(
			m_va_lib["textured_rect"],
			m_material_lib[color + "_silhoulette"],
			m_texture_lib["ship"],
			glm::vec2(BOTTOM_BORDER_HEIGHT * 0.8f, BOTTOM_BORDER_HEIGHT * 0.6f),
			pos += glm::vec2(BOTTOM_BORDER_HEIGHT, 0)));

	// score
	player.score = 0;
	player.score_indicator = CreateText(
		std::string(SCORE_DIGITS, '0'),
		color,
		{ start_x * 2, 1 - TOP_BORDER_HEIGHT / 2.f },
		{ BOTTOM_BORDER_HEIGHT * 0.8f / 2.f, BOTTOM_BORDER_HEIGHT * 0.8f});

	return std::make_shared<Player>(player);
}

//-------------------------------------------------------------
// Explosion

std::shared_ptr<Animation> SpaceInvaders::CreateExplosion(const std::shared_ptr<Gameobject>& to_explode)
{
	return std::make_shared<Animation>(
		m_va_lib["textured_rect"],
		to_explode->GetMaterial(),
		*m_animation_lib["explosion"],
		to_explode->GetSize(),
		5.f,
		to_explode->GetPos(),
		Gameobject::State::Active);
}

//-------------------------------------------------------------
// Shot Dispersion

std::shared_ptr<Animation> SpaceInvaders::CreateDispersion(const std::shared_ptr<Gameobject>& to_disperse)
{
	return std::make_shared<Animation>(
		m_va_lib["textured_rect"],
		to_disperse->GetMaterial(),
		*m_animation_lib["shot_dispersion"],
		glm::vec2(to_disperse->GetSize().y),
		5.f,
		to_disperse->GetPos(),
		Gameobject::State::Active);
}

//-------------------------------------------------------------
// Invaders

std::vector<std::shared_ptr<AnimatedObject>> SpaceInvaders::CreateInvaders()
{
	std::vector<std::shared_ptr<AnimatedObject>> invaders;
	glm::vec2 invader_tile = { 2.f / (INVADERS_IN_ROW + 2), 0.7f / INVADERS_IN_COL };
	glm::vec2 first_invader_pos = { -1 + invader_tile.x + INVADER_SIZE.x / 2, 1 - TOP_BORDER_HEIGHT - MOTHERSHIP_SIZE.y - INVADER_SIZE.y / 2 };
	for (int i = 0; i < INVADERS_IN_COL; i++)
		for (int j = 0; j < INVADERS_IN_ROW; j++)
		{
			glm::vec2 pos =
			{
				first_invader_pos.x + j * invader_tile.x,
				first_invader_pos.y - i * invader_tile.y
			};

			std::shared_ptr<AnimatedObject> invader = std::make_shared<AnimatedObject>(
				m_va_lib.Get("textured_rect"),
				m_material_lib["sprite"],
				*m_animation_lib["invader_" + std::to_string(i)],
				glm::vec2(INVADER_SIZE * (1.f + (i / 10.f))), // make the close ones lil-bigger
				0.f,
				pos);

			invaders.push_back(invader);
		}

	return invaders;
}

std::shared_ptr<Gameobject> SpaceInvaders::CreateMothership()
{
	return std::make_shared<Gameobject>(
		m_va_lib.Get("textured_rect"),
		m_material_lib["sprite"],
		m_texture_lib["mothership"],
		glm::vec2(MOTHERSHIP_SIZE), // make the close ones lil-bigger
		glm::vec2(0, 1 - TOP_BORDER_HEIGHT - MOTHERSHIP_SIZE.y/2),
		Gameobject::State::Deactivated);
}

std::shared_ptr<Gameobject> SpaceInvaders::CreateHideout()
{
	m_hideout_speed = -HIDEOUT_SPEED;
	return std::make_shared<Gameobject>(
		m_va_lib.Get("textured_rect"),
		m_material_lib["blue_silhoulette"],
		m_texture_lib["hideout"],
		glm::vec2(0.4, 0.2), // make the close ones lil-bigger
		glm::vec2(0, -1 / 2.f));
}