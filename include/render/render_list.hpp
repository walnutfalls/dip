#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

#include <render/opengl_model.hpp>

namespace render {

/***

A list of renderables is basically the scene, but to a renderer, it is basically just an iterable.
There is usually some way to iterate things to render for a pass from a scene. It might be that
iterating is done via a callback from an ecs. For now, just iterable vector.

*/

struct render_list {
    std::vector<opengl_model> items;
};
}