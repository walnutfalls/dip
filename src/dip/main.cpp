#include <os/sleeper.hpp>
#include <core/startup_config.hpp>
#include <core/glfw_context.hpp>
#include <core/input_manager.hpp>
#include <core/frame_limiter.hpp>
#include <render/renderer.hpp>
#include <render/shader_program.hpp>
#include <render/quad_mesh.hpp>
#include <render/vram_texture.hpp>
#include <render/pass.hpp>
#include <util/read_file.hpp>
#include <util/string_table.hpp>
#include <opencv2/core.hpp>


#include "ui.hpp"
#include "app.hpp"


int main() {
    util::string_table strings;

    os::sleeper sleeper;

    core::startup_config config;
    core::glfw_context context(config);
    core::input_manager input_manager(context.window());
    core::frame_timer frame_timer;
    core::frame_limiter frame_limiter(frame_timer, 30, sleeper);

    dip::app app(context, input_manager);


    render::renderer renderer;
    std::string vert = util::read_file("assets/shaders/basic.vert");
    std::string frag = util::read_file("assets/shaders/basic.frag");
    render::shader_program shader(vert, frag);
    render::pass pass(shader);
    render::quad_mesh<vertex_format::vertex_PTx> quad;

    cv::Mat& a = app.state().mat1;
    cv::Mat& b = app.state().mat2;
    render::vram_texture gpu_textures[] {
        {a.data, a.cols, a.rows},
        {b.data, b.cols, b.rows},
        {b.data, b.cols, b.rows}
    };
    render::render_list render_list;
    auto&[meshes] = render_list.items.emplace_back();
    auto& mat_mesh = meshes.emplace_back(quad.mesh);
    mat_mesh.material.textures.insert_or_assign("texture0", gpu_textures[0].gltexture());

    // https://stackoverflow.com/questions/16809833/opencv-image-loading-for-opengl-texture
    // OpenCV has opengl interop, but needs to be built with special flag, maybe that's better?
    // https://docs.opencv.org/4.x/d2/d3c/group__core__opengl.html
    auto aListener = app.aChanged.connect([&](const cv::Mat& m) {
        gpu_textures[0].rebuffer(
            m.data,
            m.rows,
            m.cols,
            (m.step & 3) ? 1 : 4,
            m.step/m.elemSize());
    });
    auto bListener = app.bChanged.connect([&](const cv::Mat& m) {
        gpu_textures[1].rebuffer(
            m.data,
            m.cols,
            m.rows,
            (m.step & 3) ? 1 : 4,
            m.step/m.elemSize());
    });
    auto outListener = app.resultChanged.connect([&](const cv::Mat& m) {
        gpu_textures[2].rebuffer(
            m.data,
            m.cols,
            m.rows,
            (m.step & 3) ? 1 : 4,
            m.step/m.elemSize());
    });


    while (!glfwWindowShouldClose(context.window())) {
        frame_timer.start();

        int width, height;
        glfwGetWindowSize(context.window(), &width, &height);
        glfwPollEvents();

        input_manager.update();

        for (const auto& tex : gpu_textures) {
            if (app.ui().bilinear) {
                tex.set_bilinear();
            } else {
                tex.set_nearest();
            }
        }

        renderer.update(width, height);
        shader.bind();

        if (app.ui().split_view) {
            gl::glViewport(0, 0, width / 2, height);
            mat_mesh.material.textures.insert_or_assign("texture0", gpu_textures[0].gltexture());
            pass.draw(render_list);

            gl::glViewport(width / 2, 0, width / 2, height);
            mat_mesh.material.textures.insert_or_assign("texture0", gpu_textures[1].gltexture());
            pass.draw(render_list);
        } else {
            gl::glViewport(0, 0, width, height);
            mat_mesh.material.textures.insert_or_assign("texture0", gpu_textures[2].gltexture());
            pass.draw(render_list);
        }

        app.ui().draw();

        context.swap_buffers();
        frame_limiter.wait_remainder();
        frame_timer.end();
    }

    return 0;
}
