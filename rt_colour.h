/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_COLOUR_H
#define RAY_TRACING_ONE_WEEK_RT_COLOUR_H

#include <stdio.h>

#include "rt_weekend.h"

// Colour layer for vec3_t
typedef vec3_t colour_t;
#define colour(r, g, b) vec3((r), (g), (b))

void rt_write_colour(FILE *stream, colour_t pixel_colour, size_t samples_per_pixel);

#endif // RAY_TRACING_ONE_WEEK_RT_COLOUR_H
