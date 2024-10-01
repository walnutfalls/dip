set (RENDERER_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/renderer/renderer.cpp
        ${PROJECT_SOURCE_DIR}/src/renderer/additional_material_property.cpp
        ${PROJECT_SOURCE_DIR}/src/renderer/program_uniforms.cpp
        ${PROJECT_SOURCE_DIR}/src/renderer/shader.cpp
        ${PROJECT_SOURCE_DIR}/src/renderer/shader_program.cpp
        ${PROJECT_SOURCE_DIR}/src/renderer/program_info.cpp
        ${PROJECT_SOURCE_DIR}/src/renderer/quad_mesh.cpp
        ${PROJECT_SOURCE_DIR}/src/renderer/vram_texture.cpp)

add_library(renderer ${RENDERER_SOURCE_FILES})
target_include_directories(renderer PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(renderer PUBLIC
        glbinding::glbinding
        glbinding::glbinding-aux
        spdlog::spdlog_header_only
        glfw)
