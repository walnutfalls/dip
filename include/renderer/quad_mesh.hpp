#pragma once

#include <renderer/opengl_mesh.hpp>
#include <data_formats/vertex_format.hpp>

namespace renderer {
    template<typename vertex_type = vertex_format::vertex_P>
    struct quad_mesh {
        quad_mesh() {
            using namespace gl;

            constexpr float vertices[] = {
                1.f, 1.f, 0.0f,
                1.f, -1.f, 0.0f,
                -1.f, -1.f, 0.0f,
                -1.f, 1.f, 0.0f
            };
            const unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
            };

            glGenVertexArrays(1, &mesh.vao);
            glBindVertexArray(mesh.vao);

            glGenBuffers(1, &mesh.vbo);
            glGenBuffers(1, &mesh.ebo);

            glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
        }

        opengl_mesh mesh;
    };

    template <>
    inline quad_mesh<vertex_format::vertex_PTx>::quad_mesh() {
        using namespace gl;

        constexpr GLint stride = sizeof(vertex_format::vertex_PTx);
        const auto offset_p = reinterpret_cast<const void *>(offsetof(vertex_format::vertex_PTx, position));
        const auto offset_tx = reinterpret_cast<const void *>(offsetof(vertex_format::vertex_PTx, texcoord));

        constexpr vertex_format::vertex_PTx vertices[4] = {
            {{1.f, 1.f, 0.0f}, {1.f, 0.f}},
            {{1.f, -1.f, 0.0f},{1.f, 1.f}},
            {{-1.f, -1.f, 0.0f}, {0.f, 1.f}},
            {{-1.f, 1.f, 0.0f}, {0.f, 0.f}},
        };
        const unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        glGenBuffers(1, &mesh.vbo);
        glGenBuffers(1, &mesh.ebo);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offset_p);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, offset_tx);
        glEnableVertexAttribArray(1);
    }
}
