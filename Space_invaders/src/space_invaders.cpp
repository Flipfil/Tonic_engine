#include "space_invaders.h"

#include <iostream>

void SpaceInvaders::Update()
{
	switch (m_state)
	{

	/* =========================  MENU LOOPS ==========================*/
	
	case State::MainMenu: // MAIN MENU

		// selection procedure
		if (input::Keyboard::PressedKey(TONIC_KEY_DOWN))
		{
			m_sound_lib["change_option"]->Play();
			m_selected_option = (m_selected_option + 1) % m_options.size();
		}
		if (input::Keyboard::PressedKey(TONIC_KEY_UP))
		{
			m_sound_lib["change_option"]->Play();
			m_selected_option = (m_selected_option + m_options.size() - 1) % m_options.size();
		}

		// confirmation
		if (input::Keyboard::PressedKey(TONIC_KEY_RETURN))
		{
			switch (m_selected_option)
			{
			case 0: NewGame(1); break;
			case 1: NewGame(2); break;
			case 2:
			default: Engine::GetInstance().Quit();
			}
		}

		// visual selection
		for (auto& option : m_options)
			for (auto& letter : option)
				letter->SetMaterial(m_material_lib["white_silhoulette"]);

		for (auto& letter : m_options[m_selected_option])
			letter->SetMaterial(m_material_lib["green_silhoulette"]);

		break;

		 
	case State::PauseMenu: // PAUSE MENU
		
		// selection
		if (input::Keyboard::PressedKey(TONIC_KEY_DOWN))
		{
			m_sound_lib["change_option"]->Play();
			m_selected_option = (m_selected_option + 1) % m_options.size();
		}
		if (input::Keyboard::PressedKey(TONIC_KEY_UP))
		{
			m_sound_lib["change_option"]->Play();
			m_selected_option = (m_selected_option + m_options.size() - 1) % m_options.size();
		}

		// confirmation
		if (input::Keyboard::PressedKey(TONIC_KEY_RETURN))
		{
			switch (m_selected_option)
			{
			case 0:  m_state = State::GameLoop; break;
			case 1:  MainMenu();                break;
			default: Engine::GetInstance().Quit();
			}
		}

		// visual
		for (auto& option : m_options)
			for (auto& letter : option)
				letter->SetMaterial(m_material_lib["white_silhoulette"]);

		for (auto& letter : m_options[m_selected_option])
			letter->SetMaterial(m_material_lib["green_silhoulette"]);

		break;

	case State::GameOver:

		//selection
		if (input::Keyboard::PressedKey(TONIC_KEY_DOWN))
		{
			m_sound_lib["change_option"]->Play();
			m_selected_option = (m_selected_option + 1) % m_options.size();
		}
		if (input::Keyboard::PressedKey(TONIC_KEY_UP))
		{
			m_sound_lib["change_option"]->Play();
			m_selected_option = (m_selected_option + m_options.size() - 1) % m_options.size();
		}

		// confirmation
		if (input::Keyboard::PressedKey(TONIC_KEY_RETURN))
		{
			switch (m_selected_option)
			{
				case 0: break;
				case 1: NewGame((int)m_players.size()); break;
				case 2: MainMenu(); break;
			}
		}

		// visual
		for (auto& option : m_options)
			for (auto& letter : option)
				letter->SetMaterial(m_material_lib["white_silhoulette"]);

		for (auto& letter : m_options[m_selected_option])
			letter->SetMaterial(m_material_lib["green_silhoulette"]);

		break;


/* ================================= GAME LOOP ======================================== */
	case State::GameLoop:

		// Pause
		if (input::Keyboard::PressedKey(TONIC_KEY_ESCAPE)) PauseMenu();

		/* -- PLAYERS -- */
		std::vector<std::shared_ptr<Player>> live_players;
		for (auto& i : m_players) if (i->lives > 0) live_players.push_back(i);

		if (live_players.empty()) GameOver();

		// movement
		for (auto& player : live_players)
		{
			float max_x = 1 - player->ship->GetSize().x / 2;
			float min_x = -1 + player->ship->GetSize().x / 2;

			if (input::Keyboard::HoldingKey(player->controls.left) && player->ship->GetPos().x > min_x)
				player->ship->Move({ -player->speed * core::Time::GetElapsed(), 0.f });
			if (input::Keyboard::HoldingKey(player->controls.right) && player->ship->GetPos().x < max_x)
				player->ship->Move({ player->speed * core::Time::GetElapsed(), 0.f });
		}

		// shooting
		for (auto& player : live_players)
		{
			if (input::Keyboard::PressedKey(player->controls.shoot) && player->shot->GetState() == Gameobject::State::Deactivated)
			{
				player->shot->SetPos(player->ship->GetPos());
				player->shot->SetState(Gameobject::State::Active);
				m_sound_lib["shoot"]->Play();
			}

			// skip inactive shot
			if (player->shot->GetState() != Gameobject::State::Active)
				continue;

			player->shot->Update();

			if (player->shot->GetPos().y > 1 - TOP_BORDER_HEIGHT)
			{
				m_explosions.push_back(CreateDispersion(player->shot));
				player->shot->SetState(Gameobject::State::Deactivated);
			}

			// shot x invader collision
			for (int i = 0; i < INVADERS_IN_COL * INVADERS_IN_ROW; i++)
			{
				auto& invader = m_invaders[i];
				if (invader->GetState() == Gameobject::State::Deactivated)
					continue;

				if (!IsColliding(player->shot->GetPos(), player->shot->GetSize(), invader->GetPos(), invader->GetSize()))
					continue; // no hit

				// deactivate objects
				player->shot->SetState(Gameobject::State::Deactivated);
				invader->SetState(Gameobject::State::Deactivated);

				// set off explosion where the invader was
				m_sound_lib["invader_explosion"]->Play();
				m_explosions.push_back(CreateExplosion(invader));

				GainScore(*player, 150 * (INVADERS_IN_COL - (i / INVADERS_IN_ROW)));
				break;
			}
		}


		/* -- INVADERS -- */

		std::vector<std::shared_ptr<AnimatedObject>> live_invaders;
		for (auto& invader : m_invaders)
			if (invader->GetState() == Gameobject::State::Active) live_invaders.push_back(invader);
		
		if (live_invaders.empty()) NextLevel();

		// animations
		for (auto& invader : live_invaders)
		{
			invader->Update();
			invader->SetFrameRate((1 - live_invaders.size() / 56.f) * 20.f); // speed up framerate - all ~~ 0.3fps, 1 ~~ 19.5fps
		}

		// movement
		m_invaders_movement_delay -= core::Time::GetElapsed();
		if (m_invaders_movement_delay < 0)
		{
			m_sound_lib["invader_move"]->Play();
			for (auto& invader : live_invaders)
				invader->Move({ m_invader_speed, 0 });
			m_invaders_movement_delay = INVADER_MOVE_DELAY * (live_invaders.size() / (55.f + 5 * (m_level-1))); // speed up proporionally to kill cnt
		}

		// screen and deadline collision
		for (auto& invader : live_invaders)
		{
			float min_x = -1.f + invader->GetSize().x / 2.f;
			float max_x =  1.f - invader->GetSize().x / 2.f;
			float min_y = -1.f + BOTTOM_BORDER_HEIGHT + PLAYER_SIZE.y + (INVADER_SIZE.y / 2.f);

			// deadline
			if (invader->GetPos().y < min_y) GameOver();

			// screen
			if ((m_invader_speed < 0 && invader->GetPos().x < min_x) ||
				(m_invader_speed > 0 && invader->GetPos().x > max_x))
			{
				m_invader_speed *= -1.f;
				for (auto& i : live_invaders) // on wall hit move all a step down and invert movement direction
					i->Move(glm::vec2(m_invader_speed, -1 * INVADER_SIZE.y * 3 / 4.f));
				break;
			}
		}

		/* -- INVADER SHOTS -- */
		// firing
		m_invaders_shoot_delay -= core::Time::GetElapsed();
		if (m_invaders_shoot_delay < 0 && m_invader_shots.size() < INVADER_MAX_SHOTS)
		{
			std::shared_ptr<Projectile> shot;
			size_t shot_type = rand() % 3;
			switch (rand() % 3)
			{
			case 0: shot = CreateBlastShot({0,-1}); 	 break;
			case 1:	shot = CreateSquigglyShot({0,-1}); break;
			case 2:	shot = CreateSpiralShot({0,-1});   break;
			}
			size_t random_invader = rand() % live_invaders.size();
			shot->SetPos(live_invaders[random_invader]->GetPos());
			shot->SetState(Gameobject::State::Active);
			m_invader_shots.push_back(shot);
			m_invaders_shoot_delay = INVADER_SHOOT_DELAY;
		}

		// Update shots
		for (auto it = m_invader_shots.begin(); it != m_invader_shots.end();)
		{
			(*it)->Update();

			// erase deactivated
			if ((*it)->GetState() == Gameobject::State::Deactivated)
				it = m_invader_shots.erase(it);
			else it++;
		}

		// collisions
		for (auto& shot : m_invader_shots)
			for (auto& player : live_players)
			{
				auto& p_shot = player->shot;
				auto& ship = player->ship;

				// shot x border collision
				if (shot->GetPos().y < -1 + BOTTOM_BORDER_HEIGHT + SHOT_SIZE.y/2.f)
				{
					shot->SetState(Gameobject::State::Deactivated);
					m_explosions.push_back(CreateDispersion(shot));
					continue;
				}

				// shot x shot collision
				if (p_shot->GetState() == Gameobject::State::Active &&
					IsColliding(shot->GetPos(), shot->GetSize(), p_shot->GetPos(), p_shot->GetSize()))
				{
					p_shot->SetState(Gameobject::State::Deactivated);
					shot->SetState(Gameobject::State::Deactivated);
					m_explosions.push_back(CreateDispersion(p_shot));
					m_explosions.push_back(CreateDispersion(shot));
					continue;
				}

				// shot x player collision
				if (IsColliding(shot->GetPos(), shot->GetSize(), ship->GetPos(), ship->GetSize()))
				{
					shot->SetState(Gameobject::State::Deactivated);
					LoseLife(*player);
					m_invader_shots.clear();
					return;
				}
			}


		/* -- MOTHERSHIP -- */

		// spawnig
		m_mothership_spawn_delay -= core::Time::GetElapsed();
		if (m_mothership_spawn_delay < 0 && m_mothership->GetState() == Gameobject::State::Deactivated)
		{
			m_mothership->SetPos({ -1 - MOTHERSHIP_SIZE.x, 1 - TOP_BORDER_HEIGHT - MOTHERSHIP_SIZE.y/2.f});
			m_mothership->SetState(Gameobject::State::Active);
			m_sound_lib["mothership_spawn"]->Play();
		}

		// movement and collision
		if (m_mothership->GetState() == Gameobject::State::Active)
		{
			// movement
			m_mothership->Move({ MOTHERSHIP_SPEED * core::Time::GetElapsed(), 0});

			// collisions with player shot
			for (auto& player : live_players)
				if (player->shot->GetState() == Gameobject::State::Active)
					if (IsColliding(
						m_mothership->GetPos(), m_mothership->GetSize(),
						player->shot->GetPos(), player->shot->GetSize()))
					{
						player->shot->SetState(Gameobject::State::Deactivated);
						m_mothership->SetState(Gameobject::State::Deactivated);
						GainScore(*player, 10000);

						m_mothership_spawn_delay = MOTHERSHIP_SPAWN_RATE;
					}
			// collision with screen
			if (m_mothership->GetPos().x > 1 + MOTHERSHIP_SIZE.x)
			{
				m_mothership_spawn_delay = MOTHERSHIP_SPAWN_RATE;
				m_mothership->SetState(Gameobject::State::Deactivated);
			}
		}


		/* -- HIDEOUT -- */

		// movement
		m_hideout->Move({ m_hideout_speed * core::Time::GetElapsed(),0 });
		if (m_hideout->GetPos().x > 0.5f && m_hideout_speed > 0)
			m_hideout_speed *= -1;
		if (m_hideout->GetPos().x < -0.5f && m_hideout_speed < 0)
			m_hideout_speed *= -1;

		// collisions
		// player shot x hideout
		for (auto& player : live_players)
			if (player->shot->GetState() == Gameobject::State::Active &&
				IsColliding(m_hideout->GetPos(), m_hideout->GetSize(), player->shot->GetPos(), player->shot->GetSize()))
			{
				player->shot->SetState(Gameobject::State::Deactivated);
				m_explosions.push_back(CreateDispersion(player->shot));
			}

		// enemy shot x hideout
		for (auto& shot : m_invader_shots)
			if (IsColliding(m_hideout->GetPos(), m_hideout->GetSize(), shot->GetPos(), shot->GetSize()))
			{
				shot->SetState(Gameobject::State::Deactivated);
				m_explosions.push_back(CreateDispersion(shot));
			}


		/* EXPLOSIONS */

		for (auto it = m_explosions.begin(); it != m_explosions.end();)
		{
			(*it)->Update();

			// erase deactivated
			if ((*it)->GetState() == Gameobject::State::Deactivated)
				it = m_explosions.erase(it);
			else it++;
		}

		break;
	}
}

//---------------------------------------------------

void SpaceInvaders::Render() // render everything as Gameobject checks its state and deactivated wont render
{
	switch (m_state)
	{
	case State::MainMenu:
	case State::PauseMenu:
	case State::GameOver:
		for (auto& letter : m_title)
			letter->Render();
		for (auto& option : m_options)
			for (auto& letter : option)
				letter->Render();
		break;

	case State::GameLoop:

		m_mothership->Render();
		m_hideout->Render();

		for (auto& invader : m_invaders)
			invader->Render();

		for (auto& explosion : m_explosions)
			explosion->Render();

		for (auto& shot : m_invader_shots)
			shot->Render();

		for (auto& player : m_players)
		{
			player->ship->Render();
			player->shot->Render();
			for (auto& letter : player->lives_indicator)
				letter->Render();
			for (auto& letter : player->score_indicator)
				letter->Render();
		}
		break;
	}
}
