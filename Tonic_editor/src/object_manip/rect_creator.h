#pragma once

#include "tonic/graphics/texture.h"
#include "tonic/graphics/shader.h"
#include "tonic/graphics/vertex.h"
#include "tonic/graphics/frame_buffer.h"

#include "external/glm/glm.hpp"

#include <memory>

using namespace tonic;

class RectCreator
{
public:
	RectCreator() 
	{

	}
	~RectCreator() {}

	inline void SetVA(std::shared_ptr<graphics::VertexArray> va) { m_va = va; }
	inline void SetTexture(std::shared_ptr<graphics::Texture> texture) { m_texture = texture; }
	inline void SetShader(std::shared_ptr<graphics::Shader> shader) { m_shader = shader; }
	inline void SetFrameBuffer() { m_frame_buffer = std::make_shared<graphics::FrameBuffer>(100,100); m_frame_buffer->SetClearColor({0.25,0.25,0.25, 1.0}); }

	void LoadFromFile(const std::string& file_name);
	void SaveToFile(const std::string& file_name);

	void Render();
	void ImguiRender();

private:

	std::shared_ptr<graphics::Shader> m_shader = nullptr;
	std::shared_ptr<graphics::VertexArray> m_va = nullptr;
	std::shared_ptr<graphics::Texture> m_texture = nullptr;
	std::shared_ptr<graphics::FrameBuffer> m_frame_buffer = nullptr;
};
