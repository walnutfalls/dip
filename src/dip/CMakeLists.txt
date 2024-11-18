
add_executable(dip
        main.cpp
        ui.cpp
        ccl.cpp
        dft.cpp
        command_interpreter.cpp
        app.cpp
        dft.hpp
        fft.cpp)

target_link_libraries(dip PRIVATE
        os
        core
        renderer
        ${OpenCV_LIBS}
        imgui::imgui
        Boost::signals2)

target_include_directories(dip PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_custom_command(
        TARGET dip PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
