#include <unordered_map>

#include <os/sleeper.hpp>

#include <core/startup_config.hpp>
#include <core/glfw_context.hpp>
#include <core/input_manager.hpp>
#include <core/frame_limiter.hpp>


#include <render/renderer.hpp>
#include <render/shader_program.hpp>
#include <render/quad_mesh.hpp>
#include <render/vram_texture.hpp>

#include <util/read_file.hpp>
#include <util/string_table.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <render/pass.hpp>

#include "ui.hpp"
#include "ccl.hpp"
#include "connectivity.hpp"
#include "command_interpreter.hpp"
#include "app.hpp"

int main() {
    util::string_table strings;

    os::sleeper sleeper;

    core::startup_config config;
    core::glfw_context context(config);
    core::input_manager input_manager(context.window());
    core::frame_timer frame_timer;
    core::frame_limiter frame_limiter(frame_timer, 60, sleeper);

    dip::app app(context.window());

    render::renderer renderer;

    std::string vert = util::read_file("assets/shaders/basic.vert");
    std::string frag = util::read_file("assets/shaders/basic.frag");
    render::shader_program shader(vert, frag);
    render::pass pass(shader);

    render::vram_texture tex1(app.a().data, app.a().cols, app.a().rows);
    render::vram_texture tex2(app.b().data, app.b().cols, app.b().rows);
    render::vram_texture tex3(app.b().data, app.b().cols, app.b().rows);

    render::quad_mesh<vertex_format::vertex_PTx> quad;


    render::render_list render_list;
    auto& model = render_list.items.emplace_back();
    auto& matmesh = model.meshes.emplace_back(quad.mesh);
    matmesh.material.textures.insert_or_assign("texture0", tex1.gltexture());


    auto aListener = app.aChanged.connect([&](const cv::Mat& m) {
        tex1.rebuffer(m.data, m.cols, m.rows);
    });
    auto bListener = app.bChanged.connect([&](const cv::Mat& m) {
        tex2.rebuffer(app.result().data, app.result().cols, app.result().rows);
    });
    auto outListener = app.resultChanged.connect([&](const cv::Mat& m) {
        tex3.rebuffer(app.result().data, app.result().cols, app.result().rows);
    });

    while (!glfwWindowShouldClose(context.window())) {
        frame_timer.start();

        glfwPollEvents();

        input_manager.update();

        if (app.ui().bilinear) {
            tex1.set_bilinear();
            tex2.set_bilinear();
            tex3.set_bilinear();
        } else {
            tex1.set_nearest();
            tex2.set_nearest();
            tex3.set_nearest();
        }

        int width, height;
        glfwGetWindowSize(context.window(), &width, &height);
        renderer.update(width, height);
        shader.bind();

        if (app.ui().operation == dip::operation::split) {
            gl::glViewport(0, 0, width / 2, height);
            matmesh.material.textures.insert_or_assign("texture0", tex1.gltexture());
            pass.draw(render_list);

            gl::glViewport(width / 2, 0, width / 2, height);
            matmesh.material.textures.insert_or_assign("texture0", tex2.gltexture());
            pass.draw(render_list);
        } else {
            gl::glViewport(0, 0, width, height);
            matmesh.material.textures.insert_or_assign("texture0", tex3.gltexture());
            pass.draw(render_list);
        }

        app.ui().draw();

        context.swap_buffers();
        frame_limiter.wait_remainder();
        frame_timer.end();
    }

    return 0;
}
