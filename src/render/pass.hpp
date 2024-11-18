#pragma once
#include <functional>

#include <render/shader_program.hpp>
#include <render/render_list.hpp>

namespace render {

    class pass final {
    public:
        explicit pass(shader_program& program);
        ~pass();

        void bind_shaders() const;
        void draw(const render_list& render_list) const;

    private:
        std::reference_wrapper<shader_program> program;

        void render_model(const opengl_model& model) const;
        void set_material(const material& material) const;

        static void draw_mesh(const opengl_mesh& mesh);

    };


}