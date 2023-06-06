#include "tonic/graphics/texture.h"

#include "tonic/graphics/helpers.h"
#include "tonic/log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"

#include "glad/glad.h"

#include <vector>

namespace tonic::graphics
{
	Texture::Texture(const std::string& file_name)
		: m_file_name(file_name)
		, m_width(0)
		, m_height(0)
		, m_num_channels(0)
		, m_pixels(nullptr)
		, m_filter(TextureFilter::Linear)
	{
		int width, height, num_channels;
		stbi_set_flip_vertically_on_load(true);
		m_pixels = stbi_load(file_name.c_str(), &width, &height, &num_channels, 0);
		if (m_pixels)
		{
			m_width = (uint32_t)width;
			m_height = (uint32_t)height;
			m_num_channels = (uint32_t)num_channels;
		}

		LoadTexture();
	}

	Texture::~Texture()
	{
		stbi_image_free(m_pixels);
		m_pixels = nullptr;
	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID); TONIC_CHECK_GL_ERROR;
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0); TONIC_CHECK_GL_ERROR;
	}

	void Texture::LoadTexture()
	{
		glGenTextures(1, &m_ID); TONIC_CHECK_GL_ERROR;
		glBindTexture(GL_TEXTURE_2D, m_ID); TONIC_CHECK_GL_ERROR;

		GLenum data_format = 0;
		switch (m_num_channels)
		{
			case 4:
				data_format = GL_RGBA; 
				break;

			case 3: 
				data_format = GL_RGB; 
				break;

			default: if (m_pixels) TONIC_ERROR("Texture format not suppored - num channels: {}", m_num_channels); break;
		}

		if (m_pixels && data_format != 0)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, data_format, m_width, m_height, 0, data_format, GL_UNSIGNED_BYTE, m_pixels); TONIC_CHECK_GL_ERROR;
			SetTextureFilter(m_filter);
			TONIC_TRACE("Loaded {}-channel texture: {}", m_num_channels, m_file_name.c_str());
		}
		else
		{
			// 8x8 white x violet (pinkish red) checker_board
			float pixels[] = {
				1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,
				0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,
				0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,
				0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,
				0.8f, 0.0f, 0.2f,     1.0f, 1.0f, 1.0f,    0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,     0.8f, 0.0f, 0.2f,    1.0f, 1.0f, 1.0f,
			};

			m_width = 8;
			m_height = 8;
			m_num_channels = 3;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, pixels); TONIC_CHECK_GL_ERROR;
			SetTextureFilter(TextureFilter::Nearest);
			TONIC_WARN("Unable to load texture: {} - defaulting to checkerboard", m_file_name.c_str());
		}

		glBindTexture(GL_TEXTURE_2D, 0); TONIC_CHECK_GL_ERROR;
	}

	void Texture::SetTextureFilter(TextureFilter filter)
	{
		m_filter = filter;

		glBindTexture(GL_TEXTURE_2D, m_ID); TONIC_CHECK_GL_ERROR;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (filter == TextureFilter::Linear) ? GL_LINEAR : GL_NEAREST); TONIC_CHECK_GL_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (filter == TextureFilter::Linear) ? GL_LINEAR : GL_NEAREST); TONIC_CHECK_GL_ERROR;
	
		glBindTexture(GL_TEXTURE_2D, 0); TONIC_CHECK_GL_ERROR;
	}

}