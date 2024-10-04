#include <render/pass.hpp>


render::pass::pass(shader_program &p) : program(p) {}

render::pass::~pass() = default;



void render::pass::bind_shaders() const {
    program.get().bind();
}

void render::pass::draw(const render_list& render_list) const {
    bind_shaders();

    for (const auto&model : render_list.items) {
        render_model(model);
    }
}


void render::pass::render_model(const opengl_model &model) const {
    for (const auto&[mesh, material] : model.meshes) {
        set_material(material);
        draw_mesh(mesh);
    }
}

void render::pass::set_material(const render::material &material) const {
    auto& prg = program.get();

    for (auto& [name, texture] : material.textures) {
        if (prg.uniform_location(name) != -1) {
            gl::glActiveTexture(gl::GLenum::GL_TEXTURE0);
            gl::glBindTexture(gl::GL_TEXTURE_2D, texture.texture_id);
        }
    }

    for (auto& [name, mat4] : material.matrix4fs) {
        prg.set_uniform(name, mat4);
    }

    for (auto& [name, vec3] : material.vector3fs) {
        prg.set_uniform(name, vec3);
    }

    for (auto& [name, vec2] : material.vector2fs) {
        prg.set_uniform(name, vec2);
    }

    for (auto& [name, vec3] : material.translation3fs) {
        prg.set_uniform(name, vec3);
    }

    for (auto& [name, f] : material.gLfloats) {
        prg.set_uniform(name, f);
    }

    for (auto& [name, i] : material.gLints) {
        prg.set_uniform(name, i);
    }

    for (auto& [name, arr] : material.array2fs) {
        prg.set_uniform(name, arr);
    }

    for (auto& [name, arr] : material.array3fs) {
        prg.set_uniform(name, arr);
    }

    for (auto& [name, arr] : material.array4fs) {
        prg.set_uniform(name, arr);
    }
}

void render::pass::draw_mesh(const opengl_mesh &mesh) {
    gl::glBindVertexArray(mesh.vao);
    gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
    gl::glBindVertexArray(0);
}
