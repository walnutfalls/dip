#pragma once

#include <Eigen/Dense>
#include <glbinding/gl/gl.h>

#include <util/debounce.hpp>

///
void gl_debug_callback(
    gl::GLenum source,
    gl::GLenum type,
    gl::GLuint id,
    gl::GLenum severity,
    gl::GLsizei length,
    const gl::GLchar* message,
    const void* user_parameter);
///

namespace render {

class renderer {
    public:
        explicit renderer();

    void update(int width, int height);

    private:
        gl::ClearBufferMask clearMask { gl::ClearBufferMask::GL_COLOR_BUFFER_BIT };
        Eigen::Vector3f clearColor { 0.2f, 0.3f, 0.3f};
};

} // dip
