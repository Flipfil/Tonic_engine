#pragma once

#include <memory>

namespace tonic::graphics
{
	class Mesh;
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

		class RenderMesh : public RenderCommand
		{
		public:
			RenderMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<Shader> shader)
				: m_mesh(mesh)
				, m_shader(shader)
			{}

			virtual void Execute() override;

		private:
			std::weak_ptr<Mesh> m_mesh;
			std::weak_ptr<Shader> m_shader;
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