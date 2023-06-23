#pragma once

#include "tonic/app.h"
#include "tonic/engine.h"

#include "tonic/core/time.h"

#include "tonic/input/keyboard.h"
#include "tonic/log.h"

#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"

#include "GameObjects/player.h"
#include "GameObjects/explosion.h"
#include "GameObjects/game_object.h"

#include "tonic/core/asset_library.h"

#define BOTTOM_BORDER_HEIGHT 1 / 8.f 
#define TOP_BORDER_HEIGHT    1 / 4.f

#define INVADER_SIZE      glm::vec2(0.1f, 0.1f * 2 / 3.f)
#define INVADER_GRID_SIZE glm::vec2(1.8f, 0.6f)
#define INVADER_GRID_POS  glm::vec2(0.f, INVADER_BOX_SIZE.y / 2.f)
#define INVADER_SPEED    0.017f
#define INVADER_TYPE_CNT 5
#define INVADER_PROJECTILE_LIMIT 5
#define INVADER_MOVE_DELAY  0.5f

#define INVADER_SHOOT_DELAY 0.5f
#define INVADER_MAX_BLASTS 4
#define INVADER_MAX_SQUIGGLIES 3
#define INVADER_MAX_SPIRALS 3



#define SHOT_SIZE			glm::vec2(0.05f * 3 / 7.f, 0.05f)
#define SHOT_BLAST_SPEED    0.01f
#define SHOT_SQUIGGLY_SPEED 0.007f
#define SHOT_SPIRAL_SPEED   0.005f


#define MAX_LIVES   3
#define MAX_PLAYERS 2

#define PLAYER_SIZE   glm::vec2(0.15f, 0.075f)
#define PLAYER_SPEED  0.01f

#define MOTHERSHIP_SIZE PLAYER_SIZE
#define MOTHERSHIP_SPEED PLAYER_SPEED

#define TEXTURES_DIR  std::string("post_build_copy/res/textures")


using namespace tonic;

class SpaceInvaders : public App
{
private:
	enum class State
	{
		MainMenu,
		GameLoop
	};

	// assets
	core::AssetLibrary<graphics::VertexArray> m_va_lib;
	core::AssetLibrary<graphics::Shader>      m_shader_lib;
	core::AssetLibrary<graphics::Texture>     m_texture_lib;
	core::AssetLibrary<graphics::Material>    m_material_lib;
	core::AssetLibrary<std::vector<std::weak_ptr<graphics::Texture>>> m_animation_lib;


	// menu objects
	std::vector<std::shared_ptr<Gameobject>> m_main_menu;

	// game loop objects
	std::vector<std::shared_ptr<AnimatedObject>> m_invaders;
	std::vector<std::shared_ptr<Gameobject>> m_invader_blasts;
	std::vector<std::shared_ptr<Gameobject>> m_invader_squigglies;
	std::vector<std::shared_ptr<Gameobject>> m_invader_spirals;
	std::vector<std::shared_ptr<Gameobject>> m_invader_explosion;

	std::vector<Player> m_players;

	// loop vars
	State m_state;

	int m_level;
	int m_player_count;

	float m_player_speed;
	float m_invader_speed;

	float m_invaders_movement_delay;
	float m_invaders_shoot_delay;

public:

	//---------------------------------------------------

	core::WindowProperties GetWindowProperties() override;

	//---------------------------------------------------

	void Initialize() override
	{
		// AssetLibs
		InitializeVALib();
		InitializeShaderLib();
		InitializeTextureAnimLib();
		InitializeMaterialLib();

		// game loop objects
		// shot prototypes
		AnimatedObject shot_blast = AnimatedObject(
			m_va_lib["textured_rect"], 
			m_material_lib["red_silhoulette"], 
			*m_animation_lib["shot_blast"],
			SHOT_SIZE, 
			10.f);
	
		AnimatedObject shot_squiggly = AnimatedObject(
			m_va_lib["textured_rect"],
			m_material_lib["yellow_silhoulette"],
			*m_animation_lib["shot_squiggly"],
			SHOT_SIZE,
			10.f);

		AnimatedObject shot_spiral = AnimatedObject(
			m_va_lib["textured_rect"],
			m_material_lib["magenta_silhoulette"],
			*m_animation_lib["shot_spiral"],
			SHOT_SIZE,
			10.f);

		// invaders shots
		for (int i = 0; i < INVADER_MAX_BLASTS;     i++) m_invader_blasts.push_back    (std::make_shared<AnimatedObject>(shot_blast));
		for (int i = 0; i < INVADER_MAX_SQUIGGLIES; i++) m_invader_squigglies.push_back(std::make_shared<AnimatedObject>(shot_squiggly));
		for (int i = 0; i < INVADER_MAX_SPIRALS;    i++) m_invader_spirals.push_back   (std::make_shared<AnimatedObject>(shot_spiral));
		
		// invaders
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				std::shared_ptr<AnimatedObject> invader = std::make_shared<AnimatedObject>(
					m_va_lib.Get("textured_rect"),
					m_material_lib["sprite"],
					*m_animation_lib["invader_" + std::to_string(i)],
					INVADER_SIZE * glm::vec2(1.f + (i / 10.f), 1.f)); // make the close ones lil-bigger 

				m_invaders.push_back(invader);
			}
		}

		// invader explosions
		for (int i = 0; i < MAX_PLAYERS; i++)
			m_invader_explosion.push_back(std::make_shared<Explosion>(
				m_va_lib["textured_rect"],
				m_material_lib["sprite"],
				*m_animation_lib["explosion"],
				INVADER_SIZE,
				5.f));

		// players
		m_players = std::vector<Player>(2);
		m_players[0].left  = TONIC_KEY_LEFT;
		m_players[0].right = TONIC_KEY_RIGHT;
		m_players[0].shoot = TONIC_KEY_SPACE;
		m_players[0].ship  = std::make_shared<Gameobject>(m_va_lib["textured_rect"], m_material_lib["green_silhoulette"], m_texture_lib["ship"], PLAYER_SIZE);
		m_players[0].lives_indicator = CreateLivesIndicator("green", 0);
		m_players[0].shot_blast = std::make_shared<AnimatedObject>(shot_blast);;
		m_players[0].shot_blast->SetMaterial(m_material_lib["green_silhoulette"]);

		m_players[1].left  = TONIC_KEY_A;
		m_players[1].right = TONIC_KEY_D;
		m_players[1].shoot = TONIC_KEY_LCTRL;
		m_players[1].ship  = std::make_shared<Gameobject>(m_va_lib["textured_rect"], m_material_lib["cyan_silhoulette"], m_texture_lib["ship"], glm::vec2(0), PLAYER_SIZE);
		m_players[1].lives_indicator = CreateLivesIndicator("cyan", 1);
		m_players[1].shot_blast = std::make_shared<AnimatedObject>(shot_blast);;
		m_players[1].shot_blast->SetMaterial(m_material_lib["cyan_silhoulette"]);


		
		// loop vars
		m_state = State::GameLoop;

		m_level = 0;
		m_player_count = MAX_PLAYERS;

		m_player_speed = PLAYER_SPEED;
		m_invader_speed = INVADER_SPEED;

		m_invaders_movement_delay = INVADER_MOVE_DELAY;
		m_invaders_shoot_delay = INVADER_SHOOT_DELAY;

		
		NewGame();
	}

	//---------------------------------------------------

	void Update() override
	{
		switch (m_state)
		{
			/* MAIN MENU */
		case State::MainMenu:
			break;

			/* GAME LOOP */
		case State::GameLoop:

			// get active invaders now so they can be checked against player blasts
			std::vector<std::shared_ptr<AnimatedObject>> active_invaders;
			for (auto& i : m_invaders)
				if (i->GetState() == Gameobject::State::Active)
					active_invaders.push_back(i);


			/* PLAYERS */ // -- have them updated first to get frame advantage

			int dead = 0;
			for (auto& player : m_players)
			{
				if (player.ship->GetState() == Gameobject::State::Deactivated)
				{
					dead++;
					continue;
				}

				// input and screen collision
				float max_x = 1 - player.ship->GetSize().x / 2;
				float min_x = -1 + player.ship->GetSize().x / 2;

				if (input::Keyboard::HoldingKey(player.left) && player.ship->GetPos().x > min_x) player.ship->Move({ -m_player_speed, 0.f });
				if (input::Keyboard::HoldingKey(player.right) && player.ship->GetPos().x < max_x) player.ship->Move({ m_player_speed, 0.f });
				if (input::Keyboard::PressedKey(player.shoot) && player.shot_blast->GetState() == Gameobject::State::Deactivated)
				{
					player.shot_blast->SetState(Gameobject::State::Active);
					player.shot_blast->SetPos(player.ship->GetPos());
				}

				if (input::Keyboard::PressedKey(TONIC_KEY_L)) LoseLife(player);

				// shooting
				if (player.shot_blast->GetState() != Gameobject::State::Active)
					continue;

				player.shot_blast->Update();
				player.shot_blast->Move({ 0,SHOT_BLAST_SPEED });

				if (player.shot_blast->GetPos().y > 1 - TOP_BORDER_HEIGHT) 
					player.shot_blast->SetState(Gameobject::State::Deactivated);

				// shot x invader collision
				for (auto& invader : active_invaders)
				{
					if (!IsColliding(player.shot_blast->GetPos(), player.shot_blast->GetSize(), invader->GetPos(), invader->GetSize()))
						continue;

					player.shot_blast->SetState(Gameobject::State::Deactivated);
					invader->SetState(Gameobject::State::Deactivated);
					for (auto& expl : m_invader_explosion)
						if (expl->GetState() == Gameobject::State::Deactivated)
						{
							expl->SetState(Gameobject::State::Active);
							expl->SetPos(invader->GetPos());
							break;
						}
					break;
				}
			}
			if (dead == m_player_count) GameOver();


			/* INVADERS */

			// animations
			for (auto& i : active_invaders)
				i->Update();

			for (auto& invader : active_invaders)
				invader->SetFrameRate((1 - active_invaders.size() / 56.f) * 20.f); // speed up framerate - all ~~ 0.3fps, 1 ~~ 19.5fps

			// movement
			m_invaders_movement_delay -= core::Time::GetElapsed();
			if (m_invaders_movement_delay < 0)
			{
				for (auto& invader : active_invaders)
					invader->Move({ m_invader_speed, 0 });
				m_invaders_movement_delay = INVADER_MOVE_DELAY * (active_invaders.size() / 55.f ); // speed up proporionally to kill cnt
			}

			// invader screen and deadline collision
			for (auto& invader : active_invaders)
			{
				float min_x = -1.f + invader->GetSize().x / 2.f;
				float max_x =  1.f - invader->GetSize().x / 2.f;
				float min_y = -1.f + BOTTOM_BORDER_HEIGHT + (PLAYER_SIZE.y + INVADER_SIZE.y) / 2;

				// deadline
				if (invader->GetPos().y < min_y) GameOver();

				// screen
				if ((m_invader_speed < 0 && invader->GetPos().x < min_x) ||
					(m_invader_speed > 0 && invader->GetPos().x > max_x))
				{
					for (auto& i : active_invaders) // on wall hit move all a step down and invert movement direction
						i->Move(glm::vec2(0.f, -1 * invader->GetSize().y * 3 / 8.f));
					m_invader_speed *= -1.f;
					break;
				}
			}

			/* INVADER SHOOTING */

			m_invaders_shoot_delay -= core::Time::GetElapsed();
			if (m_invaders_shoot_delay < 0)
			{
				std::vector<std::shared_ptr<AnimatedObject>> active_invader_shots;
				std::vector<std::shared_ptr<AnimatedObject>> inactive_invader_shots;
				for (auto& i : active_invader_shots)
				{
					if (i->GetState() == Gameobject::State::Active)
						active_invader_shots.push_back(i);
					else
						inactive_invader_shots.push_back(i);
				}
				size_t possible_shots = 1 + (active_invaders.size() / (INVADER_MAX_BLASTS + INVADER_MAX_SPIRALS + INVADER_MAX_SQUIGGLIES)); // have up to 6 active enemy shots reducing to 1 when 10 or less invaders are active
				for (int i = 0; i < )
			}

			break;
		}
	}

	//---------------------------------------------------

	void Render() override
	{
		switch (m_state)
		{
		case State::MainMenu:
			break;

		case State::GameLoop:
			for (auto& invader : m_invaders)
				invader->Render();

			for (auto& explosion : m_invader_explosion)
				explosion->Render();

			for (auto& player : m_players)
			{
				player.ship->Render();
				player.shot_blast->Render();
				for (auto& letter : player.lives_indicator)
					letter->Render();
				for (auto& letter : player.score_indicator)
					letter->Render();
			}
			break;
		}
	}

	//---------------------------------------------------

	void ImguiRender() override;

private:

	void LoseLife(Player& player)
	{
		TONIC_TRACE("LoseLife");

		// dying animation locking others
		std::weak_ptr<graphics::Material> old_mat = player.ship->GetMaterial();

		player.ship->SetMaterial(m_material_lib["red_silhoulette"]);
		player.ship->SetTexture(m_texture_lib["explosionf0"]);

		Engine::GetInstance().GetWindow().BeginRender();
		Render();
		Engine::GetInstance().GetWindow().EndRender();
		core::Time::Sleep(0.5f);

		player.ship->SetTexture(m_texture_lib["explosionf1"]);

		Engine::GetInstance().GetWindow().BeginRender();
		Render();
		Engine::GetInstance().GetWindow().EndRender();
		core::Time::Sleep(0.5f);

		player.ship->SetMaterial(old_mat);
		player.ship->SetTexture(m_texture_lib["ship"]);


		// lives indicator update
		player.lives--;
		player.lives_indicator[0]->SetTexture(m_texture_lib[std::to_string(player.lives)]);
		if (player.lives > 0)
			player.lives_indicator[player.lives]->SetState(Gameobject::State::Deactivated);

		if (player.lives <= 0)
		{
			player.ship->SetState(Gameobject::State::Deactivated);
			return;
		}

	}


	void GameOver()
	{
		TONIC_TRACE("GameOver"); 
		SaveScore();
	}

	void SaveScore() 
	{
		TONIC_TRACE("SaveScore");
	}

	void NewGame()
	{
		for (int i = 0; i < m_player_count; i++)
		{
			m_players[i].lives = MAX_LIVES;
			m_players[i].lives_indicator[0]->SetTexture(m_texture_lib[std::to_string(MAX_LIVES)]);
			m_players[i].lives_indicator[1]->SetTexture(m_texture_lib["ship"]);
			m_players[i].lives_indicator[2]->SetTexture(m_texture_lib["ship"]);

			for (auto& l : m_players[i].lives_indicator)
				l->SetState(Gameobject::State::Active);

			for (auto& l : m_players[i].score_indicator)
				l->SetState(Gameobject::State::Active);

			m_players[i].ship->SetPos({ -1 + (i + 1) * 2.f / (m_player_count + 1), -1 + BOTTOM_BORDER_HEIGHT + PLAYER_SIZE.y });
			m_players[i].ship->SetState(Gameobject::State::Active);
		}

		m_level = 1;

		std::vector<std::shared_ptr<Gameobject>> casted_invaders;
		for (auto ptr : m_invaders)
			casted_invaders.push_back(ptr);

		PlaceToGrid(
			casted_invaders,
			{ -1 + INVADER_SIZE.x + (INVADER_SIZE.x / 2.f), 1 - TOP_BORDER_HEIGHT - MOTHERSHIP_SIZE.y - (INVADER_SIZE.y / 2.f) }, 
			{INVADER_GRID_SIZE.x / 11.f, INVADER_GRID_SIZE.y / 5.f},
			11);

		for (auto& invader : m_invaders)
			invader->SetState(Gameobject::State::Active);
	}

	bool IsColliding(const glm::vec2& pos_a, const glm::vec2& size_a, const glm::vec2& pos_b, const glm::vec2& size_b)
	{
		float left_a = pos_a.x - size_a.x / 2;
		float right_a = pos_a.x + size_a.x / 2;
		float top_a = pos_a.y + size_a.y / 2;
		float bottom_a = pos_a.y - size_a.y / 2;

		float left_b   = pos_b.x - size_b.x / 2;
		float right_b  = pos_b.x + size_b.x / 2;
		float top_b    = pos_b.y + size_b.y / 2;
		float bottom_b = pos_b.y - size_b.y / 2;

		return (
			left_a < right_b && right_a > left_b &&
			top_a > bottom_b && bottom_a < top_b
			);
	}

	

	std::vector<std::shared_ptr<Gameobject>> CreateText(const std::string& str, const std::string& color, const glm::vec2& pos, const glm::vec2& letter_size)
	{
		glm::vec2 letter_position = pos;
		std::vector<std::shared_ptr<Gameobject>> text;
		for (auto c : str)
			text.push_back(std::make_shared<Gameobject>(
				m_va_lib.Get("textured_rect"),
				m_material_lib.Get(color + "_silhoulette"),
				m_texture_lib.Get(std::string(1, c)),
				letter_position += letter_size.x,
				letter_size));

		return text;
	}

	std::vector<std::shared_ptr<Gameobject>> CreateLivesIndicator(const std::string& color, int player_id)
	{
		std::vector<std::shared_ptr<Gameobject>> textures;
		textures.push_back(std::make_shared<Gameobject>(
			m_va_lib["textured_rect"],
			m_material_lib[color + "_silhoulette"],
			m_texture_lib[std::to_string(MAX_LIVES)],
			glm::vec2(0.f),
			glm::vec2(BOTTOM_BORDER_HEIGHT * 0.8f / 2.f, BOTTOM_BORDER_HEIGHT * 0.8f)));

		for (int i = 0; i < MAX_LIVES - 1; i++)
			textures.push_back(std::make_shared<Gameobject>(
				m_va_lib["textured_rect"],
				m_material_lib[color + "_silhoulette"],
				m_texture_lib["ship"],
				glm::vec2(0),
				PLAYER_SIZE * 0.8f));

		PlaceToGrid(textures, glm::vec2(player_id * -0.7f, -1), glm::vec2(0.2f, BOTTOM_BORDER_HEIGHT), MAX_LIVES);
		return textures;
	}


	void PlaceToGrid(std::vector<std::shared_ptr<Gameobject>> objects, const glm::vec2& first_obj_pos, const glm::vec2& grid_tile_size, size_t row_size)
	{
		glm::vec2 upper_left = first_obj_pos;
		glm::vec2 size = grid_tile_size;
		glm::vec2 offset = size / 2.f;
		for (int y = 0;; y++)
			for (int x = 0; x < row_size; x++)
			{
				if ((x + (y * (size_t)row_size)) >= objects.size())
					return;

				glm::vec2 pos = upper_left + offset + size * glm::vec2(x, -y);
				objects[x + (y * (size_t)row_size)]->SetPos(pos);
			}
	}

	void InitializeVALib();
	void InitializeShaderLib();
	void InitializeTextureAnimLib();
	void InitializeMaterialLib();
};