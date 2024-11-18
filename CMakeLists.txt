cmake_minimum_required(VERSION 3.14)
project(dip)

message ("${VCPKG_PLATFORM_TOOLSET}")

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")


find_package(glbinding CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(boost_signals2 REQUIRED CONFIG)


add_subdirectory(src)

