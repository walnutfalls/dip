#pragma once

#include <glbinding/gl/gl.h>

#include <unordered_map>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "opengl_texture.hpp"

namespace render {
    struct material {
        std::unordered_map<std::string, opengl_texture> textures;

        std::unordered_map<std::string, gl::GLfloat> gLfloats;
        std::unordered_map<std::string, const Eigen::Matrix4f> matrix4fs;
        std::unordered_map<std::string, const Eigen::Vector3f> vector3fs;
        std::unordered_map<std::string, const Eigen::Vector2f> vector2fs;
        std::unordered_map<std::string, gl::GLint> gLints;
        std::unordered_map<std::string, const Eigen::Translation3f> translation3fs;
        std::unordered_map<std::string, const Eigen::Array2f> array2fs;
        std::unordered_map<std::string, const Eigen::Array3f> array3fs;
        std::unordered_map<std::string, const Eigen::Array4f> array4fs;
    };

}
