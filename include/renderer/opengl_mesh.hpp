#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <glbinding/gl/gl.h>

namespace renderer
{
	struct opengl_mesh
	{
		gl::GLuint vao{ 0 };
		gl::GLuint vbo{ 0 };
		gl::GLuint ebo{ 0 };
		std::uint32_t index_count{ 0 };
	};
}
