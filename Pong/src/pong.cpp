#include "tonic/main.h"
#include "tonic/engine.h"

#include "tonic/graphics/frame_buffer.h"
#include "tonic/input/keyboard.h"
#include "tonic/log.h"

#include "factory.h"

#include "external/imgui/imgui.h"
#include "external/glm/glm.hpp"

#include <memory>

#define PADDLE_SPEED 0.05f;
#define BALL_SPEED 0.03f;


using namespace tonic;

class PongV1 : public App
{
private:
	std::shared_ptr<Gameobject> m_left_paddle, m_right_paddle;
	int m_left_paddle_down, m_left_paddle_up, m_right_paddle_up, m_right_paddle_down;
	float m_paddle_speed; 
	
	
	std::shared_ptr<Ball> m_ball;
	bool m_ball_collision_left;
	float m_ball_speed;

	bool m_playing;

	int m_left_score;
	int m_right_score;
	int m_score_to_win;

	std::string m_end_game_text;

public:
	core::WindowProperties GetWindowProperties()
	{
		core::WindowProperties props;
		props.width = 800;
		props.height = 600;
		props.imgui_props.IsDockingEnabled = true;

		return props;
	}

	void Initialize() override
	{
		m_left_paddle = factory::CreatePaddle({-1.f, 0.f});
		m_right_paddle = factory::CreatePaddle({ 1.f,0.f });

		m_left_paddle_up = TONIC_KEY_W;
		m_left_paddle_down = TONIC_KEY_S;
		m_right_paddle_up = TONIC_KEY_UP;
		m_right_paddle_down = TONIC_KEY_DOWN;


		m_ball = factory::CreateBall({ 0.f,0.f });
		m_ball_collision_left = rand() / (RAND_MAX / 2);

		m_playing = false;

		m_left_score = 0;
		m_right_score = 0;
		m_score_to_win = 5;

		m_end_game_text = "";

		Reset();
	}

	void Shutdown() override
	{
	}

	void Update() override
	{
		if (input::Keyboard::PressedKey(TONIC_KEY_ESCAPE))
		{
			Engine::GetInstance().Quit();
		}

		if (!m_playing)
			return;

		m_left_paddle->Update();
		m_right_paddle->Update();
		m_ball->Update();

		float left_max_y = 1 - m_left_paddle->GetSize().y / 2;
		float left_min_y = -1 + m_left_paddle->GetSize().y / 2;
		float right_max_y = 1 - m_right_paddle->GetSize().y / 2;
		float right_min_y = -1 + m_right_paddle->GetSize().y / 2;


		if (input::Keyboard::HoldingKey(m_left_paddle_up) && m_left_paddle->GetPos().y < left_max_y) m_left_paddle->Move({ 0.f, m_paddle_speed });
		if (input::Keyboard::HoldingKey(m_left_paddle_down) && m_left_paddle->GetPos().y > left_min_y) m_left_paddle->Move({ 0.f, -m_paddle_speed });

		if (input::Keyboard::HoldingKey(m_right_paddle_up) && m_right_paddle->GetPos().y < right_max_y) m_right_paddle->Move({ 0.f, m_paddle_speed });
		if (input::Keyboard::HoldingKey(m_right_paddle_down) && m_right_paddle->GetPos().y > right_min_y) m_right_paddle->Move({ 0.f, -m_paddle_speed });

		float ball_max_y = 1 - m_ball->GetSize().y / 2;
		float ball_min_y = -1 + m_ball->GetSize().y / 2;

		m_ball->Move(m_ball->GetVelocity());
		if (m_ball->GetPos().y > ball_max_y ||
			m_ball->GetPos().y < ball_min_y)
			m_ball->FlipVelY();

		if (IsColliding(m_left_paddle ->GetPos(), m_left_paddle ->GetSize(), m_ball->GetPos(), m_ball->GetSize()) && m_ball_collision_left)
		{
			glm::vec2 bounce = glm::normalize(m_ball->GetPos() - m_left_paddle->GetPos()) * m_ball_speed;
			m_ball->SetVelocity({m_ball_speed, bounce.y});
			m_ball_collision_left = !m_ball_collision_left;
		}
		if (IsColliding(m_right_paddle->GetPos(), m_right_paddle->GetSize(), m_ball->GetPos(), m_ball->GetSize()) && !m_ball_collision_left)
		{
			glm::vec2 bounce = glm::normalize(m_ball->GetPos() - m_right_paddle->GetPos()) * m_ball_speed;
			m_ball->SetVelocity({-m_ball_speed, bounce.y});
			m_ball_collision_left = !m_ball_collision_left;
		}

		if (m_ball->GetPos().x - m_ball->GetSize().x / 2 < -1)
		{
			m_right_score++;
			Reset();
			if (m_right_score >= m_score_to_win)
			{
				m_end_game_text = "Right player wins.";
				m_playing = false;
			}
		}
		if (m_ball->GetPos().x + m_ball->GetSize().x / 2 > 1)
		{
			m_left_score++;
			Reset();
			if (m_left_score >= m_score_to_win)
			{
				m_end_game_text = "Left player wins.";
				m_playing = false;
			}
		}
	}

	void Render() override
	{
		m_left_paddle->Render();
		m_right_paddle->Render();
		m_ball->Render();
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		if (ImGui::Begin("Game view"))
		{
			if (ImGui::IsWindowHovered())
			{
				ImGui::CaptureMouseFromApp(false);
			}

			ImVec2 size = { 500, 500 };
			ImVec2 uv0 = { 0,1 };
			ImVec2 uv1 = { 1,0 };
			ImGui::Image((void*)(intptr_t)Engine::GetInstance().GetWindow().GetFrameBuffer()->GetTextureID(), size, uv0, uv1);
		}
		ImGui::End();

		if (ImGui::Begin("Menu"))
		{
			if (!m_playing)
				if (ImGui::Button("New Game"))
				{
					m_end_game_text = "";
					m_playing = true;
					m_left_score = 0;
					m_right_score = 0;
				}
		}
		ImGui::End();


		if (ImGui::Begin("Info"))
		{
			ImGui::Text("Left  score: %d", m_left_score);
			ImGui::Text("Right score: %d", m_right_score);
			ImGui::Text("%s", m_end_game_text.c_str());
		}
		ImGui::End();
	}
private:
	bool IsColliding(const glm::vec2& pos_a, const glm::vec2& size_a, const glm::vec2& pos_b, const glm::vec2& size_b)
	{
		float left_a   = pos_a.x - size_a.x / 2;
		float right_a  = pos_a.x + size_a.x / 2;
		float top_a    = pos_a.y + size_a.y / 2;
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

	void Reset()
	{
		m_paddle_speed = PADDLE_SPEED;
		m_ball_speed = BALL_SPEED;

		float random_y_vel = m_ball_speed - (rand() * m_ball_speed * 2 / (float)RAND_MAX);
		m_ball->SetVelocity({ m_ball_collision_left ? -m_ball_speed : m_ball_speed, random_y_vel });
		m_ball->SetPos({ 0.f,0.f });
		m_left_paddle->SetPos({-1.f, 0.f});
		m_right_paddle->SetPos({1.f, 0.f });
	}
};

tonic::App* CreateApp()
{
	return new PongV1;
}