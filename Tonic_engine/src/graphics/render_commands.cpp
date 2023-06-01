#include "tonic/graphics/render_commands.h"

#include "tonic/graphics/mesh.h"
#include "tonic/graphics/shader.h"
#include "tonic/graphics/frame_buffer.h"
#include "tonic/graphics/helpers.h"

#include "tonic/engine.h"

#include "tonic/log.h"

#include "glad/glad.h"

namespace tonic::graphics::RENDER_COMMANDS
{
	void RenderMesh::Execute()
	{
		std::shared_ptr<Mesh> mesh = m_mesh.lock();
		std::shared_ptr<Shader> shader = m_shader.lock();
		if (mesh && shader)
		{
			mesh->Bind();
			shader->Bind();

			if (mesh->GetElementCount() > 0)
			{
				glDrawElements(GL_TRIANGLES, mesh->GetElementCount(), GL_UNSIGNED_INT, 0); TONIC_CHECK_GL_ERROR;
			}
			else
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount()); TONIC_CHECK_GL_ERROR;
			}


			shader->Unbind();
			mesh->Unbind();
		}
		else
		{
			TONIC_WARN("Attempting to execute RenderMesh with invalid data");
		}
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