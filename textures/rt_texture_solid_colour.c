/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_texture_solid_colour.h"
#include <rt_texture_shared.h>
#include <assert.h>

struct rt_texture_sc_s
{
    rt_texture_t base;

    colour_t colour;
};

rt_texture_sc_t *rt_texture_sc_new(double red, double green, double blue)
{
    rt_texture_sc_t *result = calloc(1, sizeof(rt_texture_sc_t));
    assert(NULL != result);

    result->colour = colour(red, green, blue);
    rt_texture_init(&result->base, RT_TEXTURE_TYPE_SOLID_COLOUR);

    return result;
}

colour_t rt_texture_sc_value(const rt_texture_sc_t *texture_sc, double u, double v, const vec3_t *p)
{
    assert(NULL != texture_sc);
    return texture_sc->colour;
}

void rt_texture_sc_delete(rt_texture_sc_t *texture_sc)
{
    if (NULL == texture_sc)
    {
        return;
    }
    free(texture_sc);
}
