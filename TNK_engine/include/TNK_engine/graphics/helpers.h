#pragma once

#include "tnk_assert.h"

#include "glad/glad.h"

namespace TNK::GRAPHICS
{
	void CheckGLError();
}

#ifdef TNK_CONFIG_DEBUG
#define TNK_CHECK_GL_ERROR TNK::GRAPHICS::CheckGLError();
#else
#define TNK_CHECK_CL_ERROR (void)0
#endif