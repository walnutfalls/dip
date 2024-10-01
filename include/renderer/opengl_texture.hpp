#pragma once

#include <glbinding/gl/gl.h>
#include <optional>

namespace renderer
{
	struct opengl_texture
	{
		gl::GLenum texture_index;
		gl::GLuint texture_id;
	};
}
