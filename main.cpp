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


int main()
{
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
    renderer::quad_mesh<vertex_format::vertex_PTx> dst_1;

    std::string img1 = cv::samples::findFile("assets/ppm/apple-20.ppm");
    std::string img2 = cv::samples::findFile("assets/ppm/cup-10.ppm");
    cv::Mat cvmat1 = cv::imread(img1, cv::IMREAD_COLOR);
    cv::Mat cvmat2 = cv::imread(img2, cv::IMREAD_COLOR);

    renderer::vram_texture tex1(cvmat1.data, cvmat1.cols, cvmat1.rows);
    renderer::vram_texture tex2(cvmat2.data, cvmat2.cols, cvmat2.rows);

    dip::ui ui(context.window());

    while(!glfwWindowShouldClose(context.window()))
    {
        frame_timer.start();

        glfwPollEvents();

        input_manager.update();

        if (ui.bilinear) {
            tex1.set_bilinear();
            tex2.set_bilinear();
        } else {
            tex1.set_nearest();
            tex2.set_nearest();
        }

        renderer.update();

        shader.bind();

        int width, height;
        glfwGetWindowSize(context.window(), &width, &height);

        gl::glViewport(0, 0, width/2, height);

        tex1.bind(gl::GL_TEXTURE0);
        gl::glBindVertexArray(src_1.mesh.vao);
        gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
        gl::glBindVertexArray(0);

        gl::glViewport(width/2, 0, width/2, height);
        tex2.bind(gl::GL_TEXTURE0);
        gl::glBindVertexArray(src_1.mesh.vao);
        gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
        gl::glBindVertexArray(0);

        ui.draw();

        context.swap_buffers();
        frame_limiter.wait_remainder();
        frame_timer.end();
    }

    return 0;
}
