#pragma once

#include "space_invaders_defines.h"

#include "tonic/app.h"
#include "tonic/engine.h"
#include "tonic/log.h"
#include "tonic/core/time.h"
#include "tonic/core/asset_library.h"
#include "tonic/input/keyboard.h"
#include "tonic/audio/sound.h"


#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"

#include "GameObjects/player.h"
#include "GameObjects/animation.h"
#include "GameObjects/projectile.h"

#include <list>

using namespace tonic;

class SpaceInvaders : public App
{
public:
	enum class State
	{
		MainMenu,
		GameLoop,
		PauseMenu,
		GameOver
	};

	

private:
	// assets
	core::AssetLibrary<graphics::VertexArray> m_va_lib;
	core::AssetLibrary<graphics::Shader>      m_shader_lib;
	core::AssetLibrary<graphics::Texture>     m_texture_lib;
	core::AssetLibrary<graphics::Material>    m_material_lib;
	core::AssetLibrary<audio::Sound>		  m_sound_lib;
	core::AssetLibrary<std::vector<std::weak_ptr<graphics::Texture>>> m_animation_lib;

	// menu objects
	std::vector<std::shared_ptr<Gameobject>> m_title;
	std::vector<std::vector<std::shared_ptr<Gameobject>>> m_options;
	size_t m_selected_option;
	int m_high_score;

	// game loop objects
	std::vector<std::shared_ptr<AnimatedObject>> m_invaders;
	std::list<std::shared_ptr<Projectile>> m_invader_shots;
	std::shared_ptr<Gameobject>			   m_mothership;
	std::shared_ptr<Gameobject>			   m_hideout;
	std::vector<std::shared_ptr<Player>>   m_players;
	std::list<std::shared_ptr<Animation>>  m_explosions;

	// loop vars
	State m_state;

	int m_level;

	float m_invader_speed;
	float m_hideout_speed;

	float m_invaders_movement_delay;
	float m_invaders_shoot_delay;

	float m_mothership_spawn_delay;

public:

	core::WindowProperties GetWindowProperties() override
	{
		core::WindowProperties props;
		props.width = 800;
		props.height = 600;
		props.aspect_ratio = 3.f / 4.f;
		props.clear_color = { 0.02f, 0.01f, 0.15f };

		return props;
	}

	void Initialize() override;
	void Update() override;
	void Render() override;
	void ImguiRender() override;

private:

	// Player functions - in space_invaders.cpp
	void GainScore(Player& player, int amount);
	void LoseLife(Player& player);
	bool IsColliding(const glm::vec2& pos_a, const glm::vec2& size_a, const glm::vec2& pos_b, const glm::vec2& size_b);

	// Game procedures
	void GameOver();
	void NewGame(int player_count);
	void NextLevel();

	// State changes - in space_invaders_states.cpp
	void SaveScore();
	void MainMenu();
	void PauseMenu();

	// factories 
	std::vector<std::shared_ptr<Gameobject>> CreateText(const std::string& str, const std::string& color, const glm::vec2& pos, const glm::vec2& letter_size);
	std::shared_ptr<Projectile> CreateBlastShot(const glm::vec2& direction);
	std::shared_ptr<Projectile> CreateSquigglyShot(const glm::vec2& direction);
	std::shared_ptr<Projectile> CreateSpiralShot(const glm::vec2& direction);
	std::shared_ptr<Player>    CreatePlayer(const std::string& color, Player::Controls controls, float start_x);
	std::shared_ptr<Animation> CreateExplosion(const std::shared_ptr<Gameobject>& to_explode);
	std::shared_ptr<Animation> CreateDispersion(const std::shared_ptr<Gameobject>& to_disperse);
	std::vector<std::shared_ptr<AnimatedObject>> CreateInvaders();
	std::shared_ptr<Gameobject> CreateMothership();
	std::shared_ptr<Gameobject> CreateHideout();


	// Asset initializators
	void InitializeVALib();
	void InitializeShaderLib();
	void InitializeTextureAnimLib();
	void InitializeMaterialLib();
	void InitializeSoundLib();
};