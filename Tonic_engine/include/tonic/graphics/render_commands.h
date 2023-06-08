#pragma once

#include <memory>

namespace tonic::graphics
{
	class VertexArray;
	class Texture;
	class Shader;
	class FrameBuffer;

	namespace RENDER_COMMANDS
	{
		class RenderCommand
		{
		public:
			virtual void Execute() = 0;
			virtual ~RenderCommand() {}
		};

		class RenderVertexArray : public RenderCommand
		{
		public:
			RenderVertexArray(std::weak_ptr<VertexArray> vertex_array, std::weak_ptr<Shader> shader)
				: m_vertex_array(vertex_array)
				, m_shader(shader)
			{}

			virtual void Execute() override;

		protected:
			std::weak_ptr<VertexArray> m_vertex_array;
			std::weak_ptr<Shader> m_shader;
		};

		class RenderVertexArrayTextured : public RenderVertexArray
		{
		public:
			RenderVertexArrayTextured(std::weak_ptr<VertexArray> vertex_array, std::weak_ptr<Texture> texture, std::weak_ptr<Shader> shader)
				: RenderVertexArray(vertex_array, shader)
				, m_texture(texture)
			{}

			virtual void Execute() override;

		private:
			std::weak_ptr<Texture> m_texture;
		};

		class PushFrameBuffer : public RenderCommand
		{
		public:
			PushFrameBuffer(std::weak_ptr<FrameBuffer> frame_buffer) : m_frame_buffer(frame_buffer) {}
			virtual void Execute() override;

		private:
			std::weak_ptr<FrameBuffer> m_frame_buffer;
		};

		class PopFrameBuffer : public RenderCommand
		{
		public:
			PopFrameBuffer() {}
			virtual void Execute() override;
		};
	}
}