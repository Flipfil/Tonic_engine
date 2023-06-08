#include "tonic/graphics/render_commands.h"

#include "tonic/graphics/vertex.h"
#include "tonic/graphics/texture.h"
#include "tonic/graphics/shader.h"
#include "tonic/graphics/frame_buffer.h"
#include "tonic/graphics/helpers.h"

#include "tonic/engine.h"
#include "tonic/log.h"

#include "glad/glad.h"

namespace tonic::graphics::RENDER_COMMANDS
{
	void RenderVertexArray::Execute()
	{
		std::shared_ptr<VertexArray> vertex_array = m_vertex_array.lock();
		std::shared_ptr<Shader> shader = m_shader.lock();
		if (!vertex_array || !shader)
		{
			TONIC_WARN("Attempting to execute RenderVertexArray with invalid data");
			return;
		}

		TONIC_ASSERT(vertex_array->IsValid(), "Attempting to execute invalid RenderVertexArray - did you forget to call VertexArray::Upload?");
		if (!vertex_array->IsValid())
			return;

		vertex_array->Bind();
		shader->Bind();

		if (vertex_array->GetElementCount() > 0)
		{
			glDrawElements(GL_TRIANGLES, vertex_array->GetElementCount(), GL_UNSIGNED_INT, 0); TONIC_CHECK_GL_ERROR;
		}
		else
		{
			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_array->GetVertexCount()); TONIC_CHECK_GL_ERROR;
		}

		shader->Unbind();
		vertex_array->Unbind();
	}

	void RenderVertexArrayTextured::Execute()
	{
		std::shared_ptr<VertexArray> vertex_array = m_vertex_array.lock();
		std::shared_ptr<Texture> texture = m_texture.lock();
		std::shared_ptr<Shader> shader = m_shader.lock();
		if (!vertex_array || !texture || !shader)
		{
			TONIC_WARN("Attempting to execute RenderVertexArrayTextured with invalid data");
			return;
		}

		TONIC_ASSERT(vertex_array->IsValid(),"Attempting to execute invalid RenderVertexArrayTextured - did you forget to call VertexArray::Upload?");
		if (!vertex_array->IsValid())
			return;

		vertex_array->Bind();
		texture->Bind();
		shader->Bind();

		if (vertex_array->GetElementCount() > 0)
		{
			glDrawElements(GL_TRIANGLES, vertex_array->GetElementCount(), GL_UNSIGNED_INT, 0); TONIC_CHECK_GL_ERROR;
		}
		else
		{
			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_array->GetVertexCount()); TONIC_CHECK_GL_ERROR;
		}


		shader->Unbind();
		texture->Unbind();
		vertex_array->Unbind();
	}

	void PushFrameBuffer::Execute()
	{
		std::shared_ptr<FrameBuffer> fb = m_frame_buffer.lock();
		if (fb)
		{
			Engine::GetInstance().GetRenderManager().PushFrameBuffer(fb);
		}
		else
		{
			TONIC_WARN("Attempting to execute PushFrameBuffer with invalid data");
		}
	}

	void PopFrameBuffer::Execute()
	{
		Engine::GetInstance().GetRenderManager().PopFrameBuffer();
	}
}