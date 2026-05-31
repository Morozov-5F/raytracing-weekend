/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_colour.h"

#include "rt_weekend.h"

#define RT_MAKE_COLOUR_COMPONENT(c) (int)(256 * rt_clamp((c), 0.0, 0.999))

void rt_write_colour(FILE *stream, colour_t pixel_colour, size_t samples_per_pixel)
{
    assert(NULL != stream);

    int r, g, b;
    rt_convert_to_rgb(pixel_colour, samples_per_pixel, &r, &g, &b);

    fprintf(stream, "%d %d %d\n", r, g, b);
}

void rt_convert_to_rgb(colour_t pixel_colour, size_t samples, int *r, int *g, int *b)
{
    double scale = 1.0 / samples;
    *r = RT_MAKE_COLOUR_COMPONENT(sqrt(scale * pixel_colour.x));
    *g = RT_MAKE_COLOUR_COMPONENT(sqrt(scale * pixel_colour.y));
    *b = RT_MAKE_COLOUR_COMPONENT(sqrt(scale * pixel_colour.z));
}
