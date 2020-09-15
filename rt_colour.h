//
// Created by morozov on 16.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_COLOUR_H
#define RAY_TRACING_ONE_WEEK_RT_COLOUR_H

#include <stdio.h>

#include "rt_vec3.h"

// Colour layer for vec3_t
typedef vec3_t colour_t;
#define colour3(x, y, z) vec3((x), (y), (z))

void rt_write_colour(FILE *stream, colour_t pixel_colour);

#endif //RAY_TRACING_ONE_WEEK_RT_COLOUR_H
