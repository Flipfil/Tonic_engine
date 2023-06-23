#include "tonic/graphics/material.h"

#include "tonic/graphics/shader.h"

#include "tonic/log.h"


namespace tonic::graphics
{
	//---------------------------------------------

	Material::Material(std::shared_ptr<Shader> shader)
		: m_shader(shader)
	{
		TONIC_ASSERT(shader, "Material::Material - Attempting to instantiate a material with null shader.");
	}


	//---------------------------------------------

	Material::Material(const Material& other)
	{
		m_shader = other.m_shader;

		// Data
		m_uniform_ints   = other.m_uniform_ints;
		m_uniform_float2 = other.m_uniform_float2;
		m_uniform_float3 = other.m_uniform_float3;
		m_uniform_float4 = other.m_uniform_float4;
		m_uniform_mat3   = other.m_uniform_mat3;
		m_uniform_mat4   = other.m_uniform_mat4;
	}

	//---------------------------------------------

	Material::~Material()
	{
	}

	//---------------------------------------------

	void Material::SetShader(std::shared_ptr<Shader> shader)
	{
		TONIC_ASSERT(shader, "Material::SetShader - Attempting to set a null shader.");
		if (shader)
			m_shader = shader;
	}
	
	//---------------------------------------------

	void Material::UpdateShaderUniforms()
	{
		if (!m_shader)
			return;
		
		for (const auto& it : m_uniform_ints)	m_shader->SetUniformInt   (it.first, it.second);
		for (const auto& it : m_uniform_floats)	m_shader->SetUniformFloat (it.first, it.second);
		for (const auto& it : m_uniform_float2)	m_shader->SetUniformFloat2(it.first, it.second);
		for (const auto& it : m_uniform_float3)	m_shader->SetUniformFloat3(it.first, it.second);
		for (const auto& it : m_uniform_float4)	m_shader->SetUniformFloat4(it.first, it.second);
		for (const auto& it : m_uniform_mat3)	m_shader->SetUniformMat3  (it.first, it.second);
		for (const auto& it : m_uniform_mat4)	m_shader->SetUniformMat4  (it.first, it.second);
	}
}