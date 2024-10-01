
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

#include <renderer/renderer.hpp>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <glbinding-aux/Meta.h>

void gl_debug_callback(
    gl::GLenum source,
    gl::GLenum type,
    gl::GLuint id,
    gl::GLenum severity,
    gl::GLsizei length,
    const gl::GLchar *message,
    const void *user_parameter) {
    using namespace gl;


    static auto print_msg = [](gl::GLenum type, gl::GLenum severity, const gl::GLchar *message) {
        switch (severity) {
            case gl::GL_DEBUG_SEVERITY_LOW:
            case gl::GLenum::GL_DEBUG_SEVERITY_MEDIUM:
                spdlog::warn(
                    "GL CALLBACK: type = {}, severity = {}, message = {}\n\n",
                    glbinding::aux::Meta::getString(type),
                    glbinding::aux::Meta::getString(severity),
                    message);
                break;
            case gl::GLenum::GL_DEBUG_SEVERITY_HIGH:
                spdlog::warn(
                    "GL CALLBACK: type = {}, severity = {}, message = {}\n\n",
                    glbinding::aux::Meta::getString(type),
                    glbinding::aux::Meta::getString(severity),
                    message);
                break;
            default:
                spdlog::info(
                    "GL CALLBACK: type = {}, severity = {}, message = {}\n\n",
                    glbinding::aux::Meta::getString(type),
                    glbinding::aux::Meta::getString(severity),
                    message);
        }
    };

    // avoid spammy non-error messages
    static debounce<gl::GLenum, gl::GLenum, const GLchar *> print_debounce(float_second(1.f), print_msg);
    print_debounce(type, severity, message);

    if (severity > GL_DEBUG_SEVERITY_NOTIFICATION || type == GL_DEBUG_TYPE_ERROR)
        print_msg(type, severity, message);
}


namespace renderer {
    renderer::renderer() {
        glbinding::initialize(glfwGetProcAddress);


        gl::glEnable(gl::GL_DEBUG_OUTPUT_SYNCHRONOUS);
        gl::glDebugMessageCallback(gl_debug_callback, nullptr);
    }

    void renderer::update(int width, int height) {
        using namespace gl;
        gl::glViewport(0, 0, width, height);
        gl::glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.f);
        gl::glClear(clearMask);
    }
} // dip
