#pragma once


namespace vertex_format
{
    struct vertex_P {
        float position[3];
    };

    struct vertex_PTx {
        float position[3];
        float texcoord[2];
    };
}