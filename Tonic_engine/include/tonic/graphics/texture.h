#pragma once

#include "external/glm/glm.hpp"

#include <string>

namespace tonic::graphics
{
	enum class TextureFilter 
	{
		Nearest, // pixelated
		Linear   // smoothed out (bad for scaling small textures)
	};

	class Texture
	{
	public:
		//--------------------------------------------------------------------
		// Texture( file_name )
		// Creates a texture from file.
		// If error occurs, a default texture is created (8x8 whiteXred checkerboard with Nearest filter)

		Texture(const std::string& file_name);

		//--------------------------------------------------------------------
		// Texture( loaded_texture, subtexture_rect{ x, y, widht, height} )
		// 
		// Creates a subtexture from texture created from file. 
		// Attempting to create a subtexture from another subtexture 
		// or incorrectly loaded texture results in a copy.
		// 
		//  - subtexture_rect: [x, y] is considered down-left corner of the subtexture
		Texture(const Texture& loaded_texture, const glm::vec4& subtexture_rect); 
			
		//--------------------------------------------------------------------
		// destructor
		~Texture();

		//--------------------------------------------------------------------
		// getters
		inline const std::string&      GetFileName() const { return	   m_file_name; }
		inline		    uint32_t             GetID() const { return		      m_ID; } // GL id
		inline		    uint32_t          GetWidth() const { return		   m_width; } // width in pixels
		inline		    uint32_t         GetHeight() const { return		  m_height; } // height in pixels
		inline		    uint32_t    GetNumChannels() const { return m_num_channels; } // image color channels
		inline	   TextureFilter  GetTextureFilter() const { return		  m_filter; } // 'zoom' filtering

		//--------------------------------------------------------------------
		// GPU binding
		void Bind();
		void Unbind();

		//--------------------------------------------------------------------
		// filter setter
		void SetTextureFilter(TextureFilter filter);

	private:
		void LoadDefault();

	private:
		std::string m_file_name;
		bool m_is_sub;
		uint32_t m_ID;
		uint32_t m_width, m_height;
		uint32_t m_num_channels;
		TextureFilter m_filter;


		unsigned char* m_pixels;
	};
}