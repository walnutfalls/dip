#ifndef __SHADER_PROGRAM_INFO__
#define __SHADER_PROGRAM_INFO__

#include <glbinding/gl/gl.h>
#include <map>
#include <string>

namespace renderer
{

	class program_info
	{
	public:
		explicit program_info(gl::GLuint programId);

		[[nodiscard]] gl::GLuint getAttribLocation(const std::string& name) const;
		[[nodiscard]] gl::GLuint getUniformLocation(const std::string& name) const;
		[[nodiscard]] gl::GLuint getUniformBlockLocation(const std::string& name) const;

		[[nodiscard]] std::uint32_t id() const { return _id; }

	private:
		std::uint32_t _id;

		int _numUniforms = 0;
		int _numAttributes = 0;
		int _numUniformBlocks = 0;

		std::map<std::string, gl::GLint> _attribLocations;
		std::map<std::string, gl::GLint> _uniformLocations;
		std::map<std::string, gl::GLuint> _uniformBlockLocations;
	};
} 

#endif