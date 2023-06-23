#pragma once

#include "external/glm/glm.hpp"

#include <unordered_map>
#include <type_traits>
#include <string>
#include <memory>

namespace tonic::graphics
{
	class Shader; 

	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader);
		Material(const Material& other);
		~Material();

		inline Shader*  GetShader()  const { return m_shader.get(); }

		void SetShader (std::shared_ptr<Shader>  shader);

		void UpdateShaderUniforms();

		template<typename T>
		inline void SetUniformValue(const std::string& name, const T& val)
		{
			if      constexpr (std::is_same<T,       int>()) {   m_uniform_ints[name] = val; }
			else if constexpr (std::is_same<T,     float>()) { m_uniform_floats[name] = val; }
			else if constexpr (std::is_same<T, glm::vec2>()) { m_uniform_float2[name] = val; }
			else if constexpr (std::is_same<T, glm::vec3>()) { m_uniform_float3[name] = val; }
			else if constexpr (std::is_same<T, glm::vec4>()) { m_uniform_float4[name] = val; }
			else if constexpr (std::is_same<T, glm::mat3>()) {   m_uniform_mat3[name] = val; }
			else if constexpr (std::is_same<T, glm::mat4>()) {   m_uniform_mat4[name] = val; }
			else static_assert(false, "Unsupported data type in Material::SetUniformValue.");
		}

#define GET_UNIFORM_VALUE(map_name, default_return) \
		const auto& it = map_name.find(name);       \
		if (it == map_name.end())					\
			return default_return;					\
		return it->second

		template<typename T>
		inline T GetUniformValue(const std::string& name) const
		{
			if      constexpr (std::is_same<T,       int>()) { GET_UNIFORM_VALUE(  m_uniform_ints,				0); }
			else if constexpr (std::is_same<T,     float>()) { GET_UNIFORM_VALUE(m_uniform_floats,			  0.f); }
			else if constexpr (std::is_same<T, glm::vec2>()) { GET_UNIFORM_VALUE(m_uniform_float2, glm::vec2(0.f)); }
			else if constexpr (std::is_same<T, glm::vec3>()) { GET_UNIFORM_VALUE(m_uniform_float3, glm::vec3(0.f)); }
			else if constexpr (std::is_same<T, glm::vec4>()) { GET_UNIFORM_VALUE(m_uniform_float4, glm::vec4(0.f)); }
			else if constexpr (std::is_same<T, glm::mat3>()) { GET_UNIFORM_VALUE(  m_uniform_mat3, glm::mat3(0.f)); }
			else if constexpr (std::is_same<T, glm::mat4>()) { GET_UNIFORM_VALUE(  m_uniform_mat4, glm::mat4(0.f)); }
			else static_assert(false, "Unsupported data type in Material::GetUniformValue.");
		}
#undef GET_UNIFORM_VALUE
		

	private:
		std::shared_ptr<Shader>  m_shader;

		std::unordered_map<std::string, int>       m_uniform_ints;
		std::unordered_map<std::string, float>	   m_uniform_floats;
		std::unordered_map<std::string, glm::vec2> m_uniform_float2;
		std::unordered_map<std::string, glm::vec3> m_uniform_float3;
		std::unordered_map<std::string, glm::vec4> m_uniform_float4;
		std::unordered_map<std::string, glm::mat3> m_uniform_mat3;
		std::unordered_map<std::string, glm::mat4> m_uniform_mat4;
	};
}