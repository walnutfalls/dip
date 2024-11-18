#pragma once

#include <stdlib.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

namespace render
{
	struct program_uniforms
	{
		gl::GLuint find_location(size_t name_hash) const;

		std::unordered_map<size_t, gl::GLuint> _name_hash_to_location;
	};
}
