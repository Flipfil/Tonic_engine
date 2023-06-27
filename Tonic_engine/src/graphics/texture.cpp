#include "tonic/graphics/texture.h"

#include "tonic/graphics/helpers.h"
#include "tonic/log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"

#include "glad/glad.h"

#include <vector>

namespace tonic::graphics
{

	//-------------------------------------------------------------

	Texture::Texture(const std::string& file_name) 
		: m_file_name(file_name)
		, m_is_sub(false)
		, m_width(0)
		, m_height(0)
		, m_num_channels(0)
		, m_pixels(nullptr)
		, m_filter(TextureFilter::Linear)
	{
		// load the image data (pixels) from file
		int width, height, num_channels;

		stbi_set_flip_vertically_on_load(true); // origin correction

		m_pixels = stbi_load(file_name.c_str(), &width, &height, &num_channels, 0);

		if (m_pixels)
		{
			m_width        = (uint32_t)width;
			m_height       = (uint32_t)height;
			m_num_channels = (uint32_t)num_channels;
		}

		// generate and bind a texture
		glGenTextures(1, &m_ID); TONIC_CHECK_GL_ERROR;
		Bind();

		// tell gl the number of channels the image has
		GLenum data_format = 0;

		switch (m_num_channels)
		{
			case 4: data_format = GL_RGBA; break;
			case 3: data_format = GL_RGB;  break;

			default: if (m_pixels) TONIC_ERROR("Texture format not suppored - num channels: {}", m_num_channels); break;
		}

		// check for correct setup
		if (m_pixels && data_format != 0)
		{
			// load the image to GPU
			glTexImage2D(GL_TEXTURE_2D, 0, data_format, m_width, m_height, 0, data_format, GL_UNSIGNED_BYTE, m_pixels); TONIC_CHECK_GL_ERROR;

			// lastly set the filter parameter
			SetTextureFilter(m_filter);
			TONIC_TRACE("Loaded {}-channel texture: {}", m_num_channels, m_file_name.c_str());
		}
		else LoadDefault();

		Unbind();
	}

	//-------------------------------------------------------------

	Texture::Texture(const Texture& loaded_texture, const glm::vec4& subtexture_rect) // [0] := x | [1] := y | [2] := width | [3] := height 
		: m_file_name(loaded_texture.GetFileName())
		, m_is_sub(true)
		, m_pixels(nullptr)
	{
		// check valid data
		bool can_create_sub = true;

		// valid parent ?
		if (!loaded_texture.m_pixels) // default texture has null m_pixels so there are no data
		{
			can_create_sub = false;
			TONIC_WARN("Creating subtexture from default or another SUBtexture is not possible.");
		}

		// valid rect ?
		if (subtexture_rect[0] > loaded_texture.m_width  || 
			subtexture_rect[1] > loaded_texture.m_height ||
			subtexture_rect[2] > loaded_texture.m_width  ||
			subtexture_rect[3] > loaded_texture.m_height )
		{
			can_create_sub = false;
			TONIC_WARN("Texture: invalid subtexture_rect: [0]={}, [1]={}, [2]={}, [3]={}.", subtexture_rect[0], subtexture_rect[1], subtexture_rect[2], subtexture_rect[3]);
		}

		m_num_channels = loaded_texture.m_num_channels;
		m_filter = loaded_texture.m_filter;

		GLenum data_format = 0;
		switch (m_num_channels)
		{
			case 4:  data_format = GL_RGBA; break;
			case 3:  data_format = GL_RGB;  break;
			default: can_create_sub = false; break;
		}

		if (!can_create_sub)
		{
			TONIC_INFO("\tCreating copy.");
			m_ID = loaded_texture.m_ID;
			m_width = loaded_texture.m_width;
			m_height = loaded_texture.m_height;
		}
		else
		{
			glGenTextures(1, &m_ID); TONIC_CHECK_GL_ERROR;
			m_width  = (uint32_t)(subtexture_rect[2]);
			m_height = (uint32_t)(subtexture_rect[3]);

			glPixelStorei(GL_UNPACK_ROW_LENGTH,  loaded_texture.m_width);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, (uint32_t)(subtexture_rect[0]));
			glPixelStorei(GL_UNPACK_SKIP_ROWS,   (uint32_t)(subtexture_rect[1]));

			Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, data_format, m_width, m_height, 0, data_format, GL_UNSIGNED_BYTE, loaded_texture.m_pixels); TONIC_CHECK_GL_ERROR;
			Unbind();

			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

			SetTextureFilter(m_filter);
		}
	}

	//-------------------------------------------------------------

	Texture::~Texture()
	{
		if (!m_is_sub)
			stbi_image_free(m_pixels);

		m_pixels = nullptr;
	}

	//-------------------------------------------------------------

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID); TONIC_CHECK_GL_ERROR;
	}

	//-------------------------------------------------------------

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0); TONIC_CHECK_GL_ERROR;
	}

	//-------------------------------------------------------------

	void Texture::SetTextureFilter(TextureFilter filter)
	{
		m_filter = filter;

		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (filter == TextureFilter::Linear) ? GL_LINEAR : GL_NEAREST); TONIC_CHECK_GL_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (filter == TextureFilter::Linear) ? GL_LINEAR : GL_NEAREST); TONIC_CHECK_GL_ERROR;
		Unbind();
	}

	//------------------------------------------------------------
	// private

	void Texture::LoadDefault() // load an 8x8 white x violet checker_board
	{
		TONIC_WARN("Unable to load texture: {} - defaulting to checkerboard", m_file_name.c_str());
		
		m_width = 8;
		m_height = 8;
		m_num_channels = 3;

		// image data
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
		
		Bind();
		// upload data to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, pixels); TONIC_CHECK_GL_ERROR;
		Unbind();

		// set filter to nearest for sharp edges 
		SetTextureFilter(TextureFilter::Nearest);
	}

	

}