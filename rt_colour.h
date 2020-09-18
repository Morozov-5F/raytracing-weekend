//
// Created by morozov on 16.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_COLOUR_H
#define RAY_TRACING_ONE_WEEK_RT_COLOUR_H

#include <stdio.h>

#include "rtweekend.h"

// Colour layer for vec3_t
typedef vec3_t colour_t;
#define colour(r, g, b) vec3((r), (g), (b))

void rt_write_colour(FILE *stream, colour_t pixel_colour, size_t samples_per_pixel);

#endif //RAY_TRACING_ONE_WEEK_RT_COLOUR_H
