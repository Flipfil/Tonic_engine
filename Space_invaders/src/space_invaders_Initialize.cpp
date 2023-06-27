#include "space_invaders.h"

#include <fstream>

// dirs
#define TEXTURES_DIR  std::string("res/textures")
#define SOUNDS_DIR  std::string("res/sound")


//----------------------------------------------------
// Initialize

void SpaceInvaders::Initialize()
{
	// AssetLibs
	InitializeVALib();
	InitializeShaderLib();
	InitializeTextureAnimLib();
	InitializeMaterialLib();
	InitializeSoundLib();

	std::ifstream high_score_file("high_score");
	if (high_score_file.is_open())
	{
		if (!(high_score_file >> m_high_score))
		{
			TONIC_ERROR("Can't read high score from file.");
			m_high_score = 0;
		}
	}
	else TONIC_ERROR("Can't open high score file.");
	high_score_file.close();

	MainMenu();
}

//----------------------------------------------------
// Materials

void SpaceInvaders::InitializeMaterialLib()
{

	std::shared_ptr<graphics::Material> red_silhoulette = std::make_shared<graphics::Material>(m_shader_lib.Get("texture_silhoulette"));
	std::shared_ptr<graphics::Material> green_silhoulette = std::make_shared<graphics::Material>(m_shader_lib.Get("texture_silhoulette"));
	std::shared_ptr<graphics::Material> blue_silhoulette = std::make_shared<graphics::Material>(m_shader_lib.Get("texture_silhoulette"));
	std::shared_ptr<graphics::Material> magenta_silhoulette = std::make_shared<graphics::Material>(m_shader_lib.Get("texture_silhoulette"));
	std::shared_ptr<graphics::Material> cyan_silhoulette = std::make_shared<graphics::Material>(m_shader_lib.Get("texture_silhoulette"));
	std::shared_ptr<graphics::Material> yellow_silhoulette = std::make_shared<graphics::Material>(m_shader_lib.Get("texture_silhoulette"));
	std::shared_ptr<graphics::Material> white_silhoulette = std::make_shared<graphics::Material>(m_shader_lib.Get("texture_silhoulette"));


	red_silhoulette->SetUniformValue("color", glm::vec3(1.f, 0.f, 0.f));
	green_silhoulette->SetUniformValue("color", glm::vec3(0.f, 1.f, 0.f));
	blue_silhoulette->SetUniformValue("color", glm::vec3(0.f, 0.f, 1.f));
	magenta_silhoulette->SetUniformValue("color", glm::vec3(1.f, 0.f, 1.f));
	cyan_silhoulette->SetUniformValue("color", glm::vec3(0.f, 1.f, 1.f));
	yellow_silhoulette->SetUniformValue("color", glm::vec3(1.f, 1.f, 0.f));
	white_silhoulette->SetUniformValue("color", glm::vec3(1.f, 1.f, 1.f));


	m_material_lib.Load("red_silhoulette", red_silhoulette);
	m_material_lib.Load("green_silhoulette", green_silhoulette);
	m_material_lib.Load("blue_silhoulette", blue_silhoulette);
	m_material_lib.Load("magenta_silhoulette", magenta_silhoulette);
	m_material_lib.Load("cyan_silhoulette", cyan_silhoulette);
	m_material_lib.Load("yellow_silhoulette", yellow_silhoulette);
	m_material_lib.Load("white_silhoulette", white_silhoulette);

	m_material_lib.Load("sprite", std::make_shared<graphics::Material>(m_shader_lib.Get("texture")));
}

//----------------------------------------------------
// VertexArrays

void SpaceInvaders::InitializeVALib()
{
	const std::vector<std::vector<float>> SQUARE_VERTICES =
	{
		{ -0.5f,  0.5f,  0.f }, // left-up
		{ -0.5f, -0.5f,  0.f }, // left-down
		{  0.5f, -0.5f,  0.f }, // right-down
		{  0.5f,  0.5f,  0.f }  // right-up
	};

	const std::vector<std::vector<short>> SQUARE_TEXTURE_COORDS =
	{
		{0,1}, // left-up
		{0,0}, // left-down
		{1,0}, // right-down
		{1,1}  // right-up
	};

	const std::vector<uint32_t> ELEMENTS =
	{
		0,3,1,
		1,3,2
	};

	std::shared_ptr<graphics::VertexArray> textured_rect_va = std::make_shared<tonic::graphics::VertexArray>();

	TONIC_CREATE_VERTEX_BUFFER(vb, float);
	TONIC_CREATE_VERTEX_BUFFER(vb_t, short);

	for (auto vertex : SQUARE_VERTICES)
		vb->PushVertex(vertex);
	vb->SetLayout({ 3 });

	for (auto texture_vertex : SQUARE_TEXTURE_COORDS)
		vb_t->PushVertex(texture_vertex);
	vb_t->SetLayout({ 2 });

	textured_rect_va->PushBuffer(std::move(vb));
	textured_rect_va->PushBuffer(std::move(vb_t));
	textured_rect_va->SetElements(ELEMENTS);

	textured_rect_va->Upload();

	m_va_lib.Load("textured_rect", textured_rect_va);
}

//----------------------------------------------------
// Textures and animations

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
		"explosion",
		"shot_dispersion"
	};

	for (const auto& f : two_framed_files)
	{
		std::shared_ptr<graphics::Texture> texture_file = std::make_shared<graphics::Texture>(TEXTURES_DIR + "/" + f + ".png");
		std::shared_ptr<graphics::Texture> frame1 = std::make_shared<graphics::Texture>(*texture_file, glm::vec4(0, 0, texture_file->GetWidth() / 2, texture_file->GetHeight()));
		std::shared_ptr<graphics::Texture> frame2 = std::make_shared<graphics::Texture>(*texture_file, glm::vec4(texture_file->GetWidth() / 2, 0, texture_file->GetWidth() / 2, texture_file->GetHeight()));

		m_texture_lib.Load(f + "f0", frame1);
		m_texture_lib.Load(f + "f1", frame2);

		m_animation_lib.Load(f, std::make_shared<std::vector<std::weak_ptr<graphics::Texture>>>(std::vector<std::weak_ptr<graphics::Texture>>({ frame1, frame2 })));
	}

	// sprites
	std::vector<std::string> full_tex_files =
	{
		"ship",
		"mothership",
		"hideout",
		"main_title"
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

//----------------------------------------------------
// Shaders

void SpaceInvaders::InitializeShaderLib()
{
	const char* VERTEX_SHADER = R"(
                #version 410 core

                layout (location = 0) in vec3 position;
                layout (location = 1) in vec2 texture_coords;

                out vec2 uvs;

                uniform mat4 model = mat4(1.0);
                void main()
                {
                    uvs = texture_coords;
                    gl_Position = model * vec4(position, 1.0);
                }
            )"; // translates vertices and passes uvs

	const char* FRAGMENT_SHADER_TEXTURE_SILHOULETTE = R"(
                #version 410 core

                in vec2 uvs;

                out vec4 out_color; 

                uniform vec3 color;
                uniform sampler2D tex;
                void main()
                {
                    out_color = vec4(color, texture(tex, uvs).a);
                }
            )"; // passes only alpha from texture and adds custom color i.e. create a silhoulette from texture and slap a new color on top

	const char* FRAGMENT_SHADER_TEXTURE = R"(
                #version 410 core

                in vec2 uvs;

                out vec4 out_color; 

                uniform sampler2D tex;
                void main()
                {
                    out_color = texture(tex, uvs);
                }
            )"; // draws texture as is

	m_shader_lib.Load("texture_silhoulette", std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, FRAGMENT_SHADER_TEXTURE_SILHOULETTE));
	m_shader_lib.Load("texture", std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, FRAGMENT_SHADER_TEXTURE));
}

//--------------------------------------------------------
// Sounds

void SpaceInvaders::InitializeSoundLib()
{
	std::vector<std::string> file_names =
	{
		"life_lost",
		"invader_explosion",
		"shoot",
		"start_level",
		"change_option",
		"game_over",
		"invader_move",
		"mothership_spawn"
	};

	for (const auto& f : file_names)
	{
		std::shared_ptr<audio::Sound> sound = std::make_shared<tonic::audio::Sound>(SOUNDS_DIR + "/" + f + ".wav");
		sound->SetupDevice();
		m_sound_lib.Load(f, sound);
	}
}