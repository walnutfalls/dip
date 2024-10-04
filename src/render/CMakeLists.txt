set (RENDERER_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/render/renderer.cpp
        ${PROJECT_SOURCE_DIR}/src/render/additional_material_property.cpp
        ${PROJECT_SOURCE_DIR}/src/render/program_uniforms.cpp
        ${PROJECT_SOURCE_DIR}/src/render/shader.cpp
        ${PROJECT_SOURCE_DIR}/src/render/shader_program.cpp
        ${PROJECT_SOURCE_DIR}/src/render/program_info.cpp
        ${PROJECT_SOURCE_DIR}/src/render/quad_mesh.cpp
        ${PROJECT_SOURCE_DIR}/src/render/vram_texture.cpp
        ${PROJECT_SOURCE_DIR}/src/render/material.cpp
        ${PROJECT_SOURCE_DIR}/src/render/pass.cpp)

add_library(renderer ${RENDERER_SOURCE_FILES})
target_include_directories(renderer PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(renderer PUBLIC
        glbinding::glbinding
        glbinding::glbinding-aux
        spdlog::spdlog_header_only
        glfw)
