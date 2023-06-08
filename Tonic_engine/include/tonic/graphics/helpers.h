#pragma once

#include "tonic/log.h"

#include "glad/glad.h"

namespace tonic::graphics
{
	void CheckGLError();
}

#ifdef TONIC_CONFIG_DEBUG
#define TONIC_CHECK_GL_ERROR tonic::graphics::CheckGLError();
#else
#define TONIC_CHECK_GL_ERROR (void)0
#endif