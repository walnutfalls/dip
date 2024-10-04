#ifndef __SHADER_PROGRAM_HPP_
#define __SHADER_PROGRAM_HPP_

#include <optional>
#include <string>
#include <glbinding/gl/gl.h>

#include <render/shader.hpp>
#include <render/program_info.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace render
{
	class shader_program
	{		
        static constexpr unsigned int GPU_INFO_BUFFER_SIZE = 512;

	public:
		shader_program(const std::string& vertex_source, const std::string& fragment_source);
		~shader_program();

		void set_uniform(const std::string& name, gl::GLfloat val) const;
		void set_uniform(const std::string& name, const Eigen::Matrix4f& mat) const;
		void set_uniform(const std::string& name, const Eigen::Vector3f& val) const;
		void set_uniform(const std::string& name, const Eigen::Vector2f& val) const;
		void set_uniform(const std::string& name, gl::GLint val) const;
		void set_uniform(const std::string& name, const Eigen::Translation3f& val) const;
		void set_uniform(const std::string& name, const Eigen::Array2f& val) const;
		void set_uniform(const std::string& name, const Eigen::Array3f& val) const;
		void set_uniform(const std::string& name, const Eigen::Array4f& val) const;


		static void set_uniform(gl::GLint loc, gl::GLfloat val);
		static void set_uniform(gl::GLint loc, const Eigen::Matrix4f& mat);
		static void set_uniform(gl::GLint loc, const Eigen::Vector3f& val);
		static void set_uniform(gl::GLint loc, const Eigen::Vector2f& val);
		static void set_uniform(gl::GLint loc, gl::GLint val);
		static void set_uniform(gl::GLint loc, const Eigen::Translation3f& val);
		static void set_uniform(gl::GLint loc, const Eigen::Array3f& val);
        static void set_uniform(gl::GLint loc, const Eigen::Array4f& val);
        static void set_mat4_array(gl::GLint location, const std::vector<Eigen::Matrix4f>& array);

		void bind_uniform_block(const std::string& name, gl::GLuint bindpoint) const;

		void bind() const;

        static void unbind();

		[[nodiscard]] gl::GLuint uniform_location(const std::string& name) const;
		[[nodiscard]] bool has_uniform(const std::string& name) const;

	private:
		shader _vertex_shader;
		shader _fragment_shader;

		gl::GLuint _id;
		std::optional<program_info> _info;

		void link();
		void validate_program();
	};
}



#endif