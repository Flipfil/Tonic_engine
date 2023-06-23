#include "space_invaders.h"

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