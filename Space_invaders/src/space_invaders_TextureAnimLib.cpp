#include "space_invaders.h"

#include <memory>


using namespace tonic;

	core::WindowProperties SpaceInvaders::GetWindowProperties()
	{
		core::WindowProperties props;
		props.width = 800;
		props.height = 600;
		props.aspect_ratio = 3.f / 4.f;
		props.clear_color = { 0.02f, 0.01f, 0.15f };

		return props;
	}
	

	//----------------------------------------------------
	// Textures

	void SpaceInvaders::InitializeTextureAnimLib()
	{
		// 2 framed animations
		std::vector<std::string> two_framed_files =
		{
			"invader_0",
			"invader_1",
			"invader_2",
			"invader_3",
			"invader_4",
			"shot_blast",
			"shot_squiggly",
			"shot_spiral",
			"explosion"
		};

		for (const auto& f : two_framed_files)
		{
			std::shared_ptr<graphics::Texture> texture_file = std::make_shared<graphics::Texture>(TEXTURES_DIR + "/" + f + ".png");
			std::shared_ptr<graphics::Texture> frame1 = std::make_shared<graphics::Texture>(*texture_file, glm::vec4(0, 0, texture_file->GetWidth() / 2, texture_file->GetHeight()));
			std::shared_ptr<graphics::Texture> frame2 = std::make_shared<graphics::Texture>(*texture_file, glm::vec4(texture_file->GetWidth() / 2, 0, texture_file->GetWidth() / 2, texture_file->GetHeight()));
			
			m_texture_lib.Load(f + "f0", frame1);
			m_texture_lib.Load(f + "f1", frame2);

			m_animation_lib.Load(f, std::make_shared<std::vector<std::weak_ptr<graphics::Texture>>>(std::vector<std::weak_ptr<graphics::Texture>>({frame1, frame2})));
		}

		// sprites
		std::vector<std::string> full_tex_files =
		{
			"ship",
			"mothership",
			"hideout"
		};

		for (const auto& f : full_tex_files)
			m_texture_lib.Load(f, std::make_shared<graphics::Texture>(TEXTURES_DIR + "/" + f + ".png"));

		// font
		m_texture_lib.Load("font", std::make_shared<graphics::Texture>(TEXTURES_DIR + "/font.png"));

		const glm::vec2 letter_dims = { 5, 10 };
		glm::vec2 letter_pos = { 0, 0 };

		// special chars 
		std::string spec_chars = "?!.,:;()\"-+=<># ";
		for (auto c : spec_chars)
		{
			m_texture_lib.Load(std::string(1, c), std::make_shared<graphics::Texture>(*m_texture_lib.Get("font"), glm::vec4(letter_pos, letter_dims)));
			letter_pos.x += letter_dims.x;
		}

		// numbers and alphabet
		letter_pos = { 0, letter_pos.y + letter_dims.y };

		auto LoadLetters = [&](char first, char last, glm::vec2& first_letter_down_left_pixel, const glm::vec2 letter_dims)
		{
			glm::vec2 pos = first_letter_down_left_pixel;
			for (char c = first; c <= last; c++)
			{
				m_texture_lib.Load(std::string(1, c), std::make_shared<graphics::Texture>(*m_texture_lib["font"], glm::vec4(pos, letter_dims)));
				pos.x += letter_dims.x;
			}
			first_letter_down_left_pixel = { 0, pos.y + letter_dims.y };
		};

		LoadLetters('0', '9', letter_pos, letter_dims);
		LoadLetters('a', 'z', letter_pos, letter_dims);
		LoadLetters('A', 'Z', letter_pos, letter_dims);
		
		// set pixel filter
		for (auto& [key, val] : m_texture_lib.GetAll())
			val->SetTextureFilter(tonic::graphics::TextureFilter::Nearest);
	}


