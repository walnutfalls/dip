#pragma once

#include <renderer/opengl_texture.hpp>

namespace renderer {
    class vram_texture {
    public:
        vram_texture(
            std::uint8_t* data,
            std::size_t width,
            std::size_t height,
            gl::GLenum format = gl::GL_RGB);

        [[nodiscard]] const opengl_texture& gltexture() const {  return _texture; }

        void bind(gl::GLenum unit) const;
        void set_bilinear() const;
        void set_nearest() const;

    private:
        opengl_texture _texture;
    };
}
