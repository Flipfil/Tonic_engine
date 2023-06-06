#pragma once

#include <string>

namespace tonic::graphics
{
	enum class TextureFilter 
	{
		Nearest,
		Linear
	};

	class Texture
	{
	public:
		Texture(const std::string& file_name);
		~Texture();

		inline uint32_t GetID() const { return m_ID; }
		inline uint32_t GetWidth() const { return m_width; }
		inline uint32_t GetHeight() const { return m_height; }
		inline uint32_t GetNumChannels() const { return m_num_channels; }
		inline const std::string& GetFileName() const { return m_file_name; }
		inline TextureFilter GetTextureFilter() const { return m_filter; }



		void Bind();
		void Unbind();

		void SetTextureFilter(TextureFilter filter);

	private:
		void LoadTexture();

	private:
		std::string m_file_name;
		uint32_t m_ID;
		uint32_t m_width, m_height;
		uint32_t m_num_channels;
		TextureFilter m_filter;


		unsigned char* m_pixels;
	};
}