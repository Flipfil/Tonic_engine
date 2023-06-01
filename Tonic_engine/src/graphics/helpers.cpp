#include "tonic/graphics/helpers.h"

namespace tonic::graphics
{
	void CheckGLError()
	{
		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			std::string error_msg;
			switch (error)
			{
			case GL_INVALID_OPERATION:			   error_msg = "INVALID_OPERATION"; break;
			case GL_INVALID_ENUM:				   error_msg = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:				   error_msg = "INVALID_VALUE"; break;
			case GL_OUT_OF_MEMORY:				   error_msg = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error_msg = "INVALID_FRAMEBUFFER_OPERATION"; break;
			default:							   error_msg = std::to_string(error); break;
			}

			TONIC_ERROR("OpenGL Error: {}", error_msg.c_str());
			error = glGetError();
			TONIC_ASSERT(false, "OpenGL Error!");
		}
	}
}