#include <os/sleeper.hpp>

#include <core/startup_config.hpp>
#include <core/glfw_context.hpp>
#include <core/input_manager.hpp>
#include <core/frame_limiter.hpp>


#include <renderer/renderer.hpp>
#include <renderer/shader_program.hpp>
#include <renderer/quad_mesh.hpp>
#include <renderer/vram_texture.hpp>

#include <util/read_file.hpp>
#include <util/string_table.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "ui.hpp"


int main() {
    util::string_table strings;

    os::sleeper sleeper;

    core::startup_config config;
    core::glfw_context context(config);
    core::input_manager input_manager(context.window());
    core::frame_timer frame_timer;
    core::frame_limiter frame_limiter(frame_timer, 60, sleeper);

    renderer::renderer renderer;

    std::string vert = util::read_file("assets/shaders/basic.vert");
    std::string frag = util::read_file("assets/shaders/basic.frag");
    renderer::shader_program shader(vert, frag);

    renderer::quad_mesh<vertex_format::vertex_PTx> src_1;

    std::string img1 = cv::samples::findFile("assets/ppm/apple-20.ppm");
    std::string img2 = cv::samples::findFile("assets/ppm/cup-10.ppm");
    cv::Mat cvmat1 = cv::imread(img1, cv::IMREAD_COLOR);
    cv::Mat cvmat2 = cv::imread(img2, cv::IMREAD_COLOR);

    renderer::vram_texture tex1(cvmat1.data, cvmat1.cols, cvmat1.rows);
    renderer::vram_texture tex2(cvmat2.data, cvmat2.cols, cvmat2.rows);
    renderer::vram_texture tex3(cvmat2.data, cvmat2.cols, cvmat2.rows);

    dip::ui ui(context.window());
    auto ppm1Listener = ui.ppm1Changed.connect([&](const std::string &path) {
        cvmat1 = cv::imread(path, cv::IMREAD_COLOR);
        tex1.rebuffer(cvmat1.data, cvmat1.cols, cvmat1.rows);
    });

    auto ppm2Listener = ui.ppm2Changed.connect([&](const std::string &path) {
        cvmat2 = cv::imread(path, cv::IMREAD_COLOR);
        tex2.rebuffer(cvmat2.data, cvmat2.cols, cvmat2.rows);
    });

    auto opListener = ui.opChanged.connect([&](const dip::operation op) {
        const auto max_width = std::max(cvmat1.cols, cvmat2.cols);
        const auto max_height = std::max(cvmat1.rows, cvmat2.rows);

        cv::Mat a,b;
        cv::copyMakeBorder(cvmat1,a, 0,max_height - cvmat1.rows,0,max_width - cvmat1.cols,cv::BORDER_CONSTANT,0);
        cv::copyMakeBorder(cvmat2,b,0,max_height - cvmat2.rows,0,max_width - cvmat2.cols,cv::BORDER_CONSTANT,0);

        switch (op) {
            case dip::operation::add: {
                cv::Mat m = a + b;
                tex3.rebuffer(m.data, m.cols, m.rows);
            }
            break;
            case dip::operation::sub: {
                const cv::Mat m = a - b;
                tex3.rebuffer(m.data, m.cols, m.rows);
            }
            break;
            case dip::operation::mul: {
                const cv::Mat m = a.mul(b);
                tex3.rebuffer(m.data, m.cols, m.rows);
            }
            break;
            case dip::operation::negate: {
                const cv::Mat m;
                bitwise_not(a, b);
                tex3.rebuffer(b.data, b.cols, b.rows);
            }
            break;
            case dip::operation::log: {
                cv::Mat m;
                a.convertTo(m, CV_32F);
                cv::log(m + 1, m);
                m = ui.log_c * m;
                m.convertTo(a, CV_8U);
                tex3.rebuffer(a.data, a.cols, a.rows);
            }
            break;
            case dip::operation::gamma: {
                cv::Mat m;
                a.convertTo(m, CV_32F);
                cv::pow(m, ui.gamma_val, m);
                m = ui.gamma_c * m;
                m.convertTo(a, CV_8U);
                tex3.rebuffer(a.data, a.cols, a.rows);
            }
            break;
            default:
                break;
        }
    });

    while (!glfwWindowShouldClose(context.window())) {
        frame_timer.start();

        glfwPollEvents();

        input_manager.update();

        if (ui.bilinear) {
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

        if (ui.operation == dip::operation::split) {
            gl::glViewport(0, 0, width / 2, height);
            tex1.bind(gl::GL_TEXTURE0);
            gl::glBindVertexArray(src_1.mesh.vao);
            gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
            gl::glBindVertexArray(0);

            gl::glViewport(width / 2, 0, width / 2, height);
            tex2.bind(gl::GL_TEXTURE0);
            gl::glBindVertexArray(src_1.mesh.vao);
            gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
            gl::glBindVertexArray(0);
        } else {
            gl::glViewport(0, 0, width, height);
            tex3.bind(gl::GL_TEXTURE0);
            gl::glBindVertexArray(src_1.mesh.vao);
            gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
            gl::glBindVertexArray(0);
        }

        ui.draw();

        context.swap_buffers();
        frame_limiter.wait_remainder();
        frame_timer.end();
    }

    return 0;
}
