#pragma once

#include "external/glm/glm.hpp"

#include <memory>

namespace tonic::graphics
{
	class VertexArray;
	class Texture;
	class Shader;
	class FrameBuffer;
	class Camera;
	class Material;

	namespace RENDER_COMMANDS
	{
		//--------------------------------------------
		// Abstract command
		class RenderCommand
		{
		public:
			virtual void Execute() = 0;
			virtual ~RenderCommand() {}
		};

		//--------------------------------------------

		class RenderVertexArray : public RenderCommand
		{
		public:
			RenderVertexArray(std::weak_ptr<VertexArray> vertex_array, std::weak_ptr<Shader> shader, const glm::mat4& model_matrix = glm::mat4(1.0f))
				: m_vertex_array(vertex_array)
				, m_shader(shader)
				, m_model_matrix(model_matrix)
			{}

			virtual void Execute() override;

		protected:
			std::weak_ptr<VertexArray> m_vertex_array;
			std::weak_ptr<Shader> m_shader;
			glm::mat4 m_model_matrix;
		};

		//--------------------------------------------

		class RenderVertexArrayTextured : public RenderVertexArray
		{
		public:
			RenderVertexArrayTextured(std::weak_ptr<VertexArray> vertex_array, std::weak_ptr<Texture> texture, std::weak_ptr<Shader> shader, const glm::mat4& model_matrix = glm::mat4(1.0f))
				: RenderVertexArray(vertex_array, shader, model_matrix)
				, m_texture(texture)
			{}

			virtual void Execute() override;

		private:
			std::weak_ptr<Texture> m_texture;
		};

		//--------------------------------------------

		class RenderVertexArrayMaterial : public RenderCommand
		{
		public:
			RenderVertexArrayMaterial(std::weak_ptr<VertexArray> vertex_array, std::weak_ptr<Material> material, const glm::mat4& model_matrix = glm::mat4(1.0f))
				: m_vertex_array(vertex_array)
				, m_material(material)
				, m_model_matrix(model_matrix)
			{}

			virtual void Execute() override;

		protected:
			std::weak_ptr<VertexArray> m_vertex_array;
			std::weak_ptr<Material> m_material;
			glm::mat4 m_model_matrix;
		};

		//--------------------------------------------

		class RenderVertexArrayMaterialTextured : public RenderVertexArrayMaterial
		{
		public:
			RenderVertexArrayMaterialTextured(std::weak_ptr<VertexArray> vertex_array, std::weak_ptr<Material> material, std::weak_ptr<Texture> texture, const glm::mat4& model_matrix = glm::mat4(1.0f))
				: RenderVertexArrayMaterial(vertex_array, material, model_matrix)
				, m_texture(texture)
			{}

			virtual void Execute() override;

		private:
			std::weak_ptr<Texture> m_texture;
		};

		//--------------------------------------------

		class PushFrameBuffer : public RenderCommand
		{
		public:
			PushFrameBuffer(std::weak_ptr<FrameBuffer> frame_buffer) : m_frame_buffer(frame_buffer) {}
			virtual void Execute() override;

		private:
			std::weak_ptr<FrameBuffer> m_frame_buffer;
		};

		//--------------------------------------------

		class PopFrameBuffer : public RenderCommand
		{
		public:
			PopFrameBuffer() {}
			virtual void Execute() override;
		};

		//--------------------------------------------

		class PushCamera : public RenderCommand
		{
		public:
			PushCamera(std::weak_ptr<Camera> camera) : m_camera(camera) {}
			virtual void Execute() override;
		private:
			std::weak_ptr<Camera> m_camera;
		};

		//--------------------------------------------

		class PopCamera : public RenderCommand
		{
		public:
			PopCamera() {}
			virtual void Execute() override;
		};
	}
}