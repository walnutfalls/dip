#include <render/vram_texture.hpp>


render::vram_texture::vram_texture(
    const std::uint8_t* data,
    const gl::GLsizei width,
    const gl::GLsizei height,
    const gl::GLenum format) {
    using namespace gl;
    glGenTextures(1, &_texture.texture_id);
    glBindTexture(GL_TEXTURE_2D, _texture.texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void render::vram_texture::rebuffer(
    const std::uint8_t *data,
    const gl::GLsizei width,
    const gl::GLsizei height,
    const gl::GLint unpack_alignment,
    const gl::GLint unpack_row_len,
    const gl::GLenum format) const {
    using namespace gl;
    glBindTexture(GL_TEXTURE_2D, _texture.texture_id);

    gl::glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);
    gl::glPixelStorei(GL_UNPACK_ROW_LENGTH, unpack_row_len);


    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void render::vram_texture::bind(const gl::GLenum unit) const {
    using namespace gl;
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _texture.texture_id);
}

void render::vram_texture::set_bilinear() const {
    using namespace gl;
    glBindTexture(GL_TEXTURE_2D, _texture.texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void render::vram_texture::set_nearest() const {
    using namespace gl;
    bind(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
