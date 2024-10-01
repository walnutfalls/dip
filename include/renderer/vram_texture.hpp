#pragma once

#include <renderer/opengl_texture.hpp>

namespace renderer {
    class vram_texture {
    public:
        vram_texture(
            const std::uint8_t* data,
            gl::GLsizei width,
            gl::GLsizei height,
            gl::GLenum format = gl::GL_RGB);

        [[nodiscard]] const opengl_texture& gltexture() const {  return _texture; }

        void rebuffer(
            const std::uint8_t* data,
            gl::GLsizei width,
            gl::GLsizei height,
            gl::GLenum format = gl::GL_RGB) const;

        void bind(gl::GLenum unit) const;
        void set_bilinear() const;
        void set_nearest() const;

    private:
        opengl_texture _texture{};
    };
}
