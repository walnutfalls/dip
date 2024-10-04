#pragma once

#include <render/opengl_mesh.hpp>
#include <vector>
#include <render/material.hpp>


namespace render {
    struct material_mesh {
        explicit material_mesh(const opengl_mesh& other) : mesh(other) {}
        explicit material_mesh(const opengl_mesh&& other) : mesh(other) {}

        opengl_mesh mesh;
        material material;
    };

    struct opengl_model {
        std::vector<material_mesh> meshes;
    };
}
