#include "tonic/graphics/shader.h"
#include "tonic/graphics/helpers.h"
#include "tonic/log.h"

#include "glad/glad.h"

namespace tonic::graphics
{
	Shader::Shader(const std::string& vertex, const std::string& fragment)
	{
		m_program_id = glCreateProgram();

		int status = GL_FALSE;
		char error_log[512];

		uint32_t vertex_shader_id = glCreateShader(GL_VERTEX_SHADER); TONIC_CHECK_GL_ERROR;

		{
			const GLchar* gl_source = vertex.c_str();
			glShaderSource(vertex_shader_id, 1, &gl_source, NULL); TONIC_CHECK_GL_ERROR;
			glCompileShader(vertex_shader_id); TONIC_CHECK_GL_ERROR;
			glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &status); TONIC_CHECK_GL_ERROR;
			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(vertex_shader_id, sizeof(error_log), NULL, error_log); TONIC_CHECK_GL_ERROR;
				TONIC_ERROR("Vertex shader compilation error: {}", error_log);
			}
			else
			{
				glAttachShader(m_program_id, vertex_shader_id); TONIC_CHECK_GL_ERROR;
			}
		}

		uint32_t fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER); TONIC_CHECK_GL_ERROR;
		if (status == GL_TRUE)
		{
			const GLchar* gl_source = fragment.c_str();
			glShaderSource(fragment_shader_id, 1, &gl_source, NULL); TONIC_CHECK_GL_ERROR;
			glCompileShader(fragment_shader_id); TONIC_CHECK_GL_ERROR;
			glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &status); TONIC_CHECK_GL_ERROR;
			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(fragment_shader_id, sizeof(error_log), NULL, error_log); TONIC_CHECK_GL_ERROR;
				TONIC_ERROR("Fragment shader compilation error: {}", error_log);
			}
			else
			{
				glAttachShader(m_program_id, fragment_shader_id); TONIC_CHECK_GL_ERROR;
			}
		}

		TONIC_ASSERT(status == GL_TRUE, "Error compiling shader");

		if (status == GL_TRUE)
		{
			glLinkProgram(m_program_id); TONIC_CHECK_GL_ERROR;
			glValidateProgram(m_program_id); TONIC_CHECK_GL_ERROR;
			glGetProgramiv(m_program_id, GL_LINK_STATUS, &status); TONIC_CHECK_GL_ERROR;
			if (status != GL_TRUE)
			{
				glGetProgramInfoLog(m_program_id, sizeof(error_log), NULL, error_log); TONIC_CHECK_GL_ERROR;
				TONIC_ERROR("Shader link error: {}", error_log); 
				glDeleteProgram(m_program_id); TONIC_CHECK_GL_ERROR;
				m_program_id = -1;
			}
		}

		glDeleteShader(vertex_shader_id); TONIC_CHECK_GL_ERROR;
		glDeleteShader(fragment_shader_id); TONIC_CHECK_GL_ERROR;
	}


	Shader::~Shader()
	{
		glUseProgram(0); TONIC_CHECK_GL_ERROR;
		glDeleteProgram(m_program_id); TONIC_CHECK_GL_ERROR;
	}

	void Shader::Bind()
	{
		glUseProgram(m_program_id); TONIC_CHECK_GL_ERROR;
	}

	void Shader::Unbind()
	{
		glUseProgram(0); TONIC_CHECK_GL_ERROR;
	}

	void Shader::SetUniformInt(const std::string& name, int val)
	{
		glUseProgram(m_program_id); TONIC_CHECK_GL_ERROR;
		glUniform1i(GetUniformLocation(name), val); TONIC_CHECK_GL_ERROR;
	}
	
	void Shader::SetUniformFloat(const std::string& name, float val)
	{
		glUseProgram(m_program_id); TONIC_CHECK_GL_ERROR;
		glUniform1f(GetUniformLocation(name), val); TONIC_CHECK_GL_ERROR;
	}

	void Shader::SetUniformFloat2(const std::string& name, float val1, float val2)
	{
		glUseProgram(m_program_id); TONIC_CHECK_GL_ERROR;
		glUniform2f(GetUniformLocation(name), val1, val2); TONIC_CHECK_GL_ERROR;
	}

	void Shader::SetUniformFloat3(const std::string& name, float val1, float val2, float val3)
	{
		glUseProgram(m_program_id); TONIC_CHECK_GL_ERROR;
		glUniform3f(GetUniformLocation(name), val1, val2, val3); TONIC_CHECK_GL_ERROR;
	}

	void Shader::SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4)
	{
		glUseProgram(m_program_id); TONIC_CHECK_GL_ERROR;
		glUniform4f(GetUniformLocation(name), val1, val2, val3, val4); TONIC_CHECK_GL_ERROR;
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		auto it = m_uniform_locations.find(name);
		if (it == m_uniform_locations.end())
			m_uniform_locations[name] = glGetUniformLocation(m_program_id, name.c_str()); TONIC_CHECK_GL_ERROR;

		return m_uniform_locations[name];
	}
}

