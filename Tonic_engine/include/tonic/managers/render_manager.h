#pragma once

#include "tonic/graphics/render_commands.h"

#include "external/glm/glm.hpp"

#include <memory>
#include <queue>
#include <stack>

#define TONIC_SUBMIT_RENDER_CMD(type, ...) std::move(std::make_unique<tonic::graphics::RENDER_COMMANDS::type>(__VA_ARGS__))

namespace tonic::managers
{
	class RenderManager
	{
		friend class graphics::RENDER_COMMANDS::PushFrameBuffer;
		friend class graphics::RENDER_COMMANDS::PopFrameBuffer;
		friend class graphics::RENDER_COMMANDS::PushCamera;
		friend class graphics::RENDER_COMMANDS::PopCamera;

	public:
		RenderManager() {}
		~RenderManager() {}

		inline const graphics::Camera* GetActiveCamera() const { return m_cameras.empty() ? nullptr : m_cameras.top().get(); }

		void Initialize();
		void Shutdown();

		void Clear();
		void SetClearColor(const glm::vec4 clear_color);
		void SetViewPort(const glm::ivec4 dimensions);
		void Submit(std::unique_ptr<graphics::RENDER_COMMANDS::RenderCommand> rc);

		//Exexute submitted RenderCoins in the order they were recieved.
		void Flush();


		void SetWireframeMode(bool enabled);

	private:
		void PushFrameBuffer(std::shared_ptr<graphics::FrameBuffer> fb);
		void PopFrameBuffer();
		void PushCamera(std::shared_ptr<graphics::Camera> camera);
		void PopCamera();

	private:
		std::queue<std::unique_ptr<graphics::RENDER_COMMANDS::RenderCommand>> m_render_commands;
		std::stack<std::shared_ptr<graphics::FrameBuffer>> m_frame_buffers;
		std::stack<std::shared_ptr<graphics::Camera>> m_cameras;
	};
}