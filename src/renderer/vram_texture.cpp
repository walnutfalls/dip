#include <renderer/vram_texture.hpp>


renderer::vram_texture::vram_texture(
        std::uint8_t* data,
        std::size_t width,
        std::size_t height,
        gl::GLenum format) {
    using namespace gl;
    gl::glGenTextures(1, &_texture.texture_id);
    gl::glBindTexture(GL_TEXTURE_2D, _texture.texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void renderer::vram_texture::bind(gl::GLenum unit) const {
    using namespace gl;
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _texture.texture_id);

}

void renderer::vram_texture::set_bilinear() const {
    using namespace gl;
    bind(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void renderer::vram_texture::set_nearest() const {
    using namespace gl;
    bind(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
