#include "graphics/render_commands.h"
#include "log.h"

#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/helpers.h"

#include "glad/glad.h"

namespace TNK::GRAPHICS::RENDER_COMMANDS
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
				glDrawElements(GL_TRIANGLES, mesh->GetElementCount(), GL_UNSIGNED_INT, 0); TNK_CHECK_GL_ERROR;
			}
			else
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount()); TNK_CHECK_GL_ERROR;
			}


			shader->Unbind();
			mesh->Unbind();
		}
		else
		{
			TNK_WARN("Attempting to execute RenderMesh with invalid data");
		}
	}
}