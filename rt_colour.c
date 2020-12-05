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

    double r = pixel_colour.x;
    double g = pixel_colour.y;
    double b = pixel_colour.z;

    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    fprintf(stream, "%d %d %d\n", RT_MAKE_COLOUR_COMPONENT(r), RT_MAKE_COLOUR_COMPONENT(g),
            RT_MAKE_COLOUR_COMPONENT(b));
}
