/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_texture_checker_pattern.h"
#include <rt_texture_shared.h>
#include <assert.h>
#include <rt_texture_solid_colour.h>

struct rt_texture_cp_s
{
    rt_texture_t base;

    rt_texture_t *even;
    rt_texture_t *odd;
};

rt_texture_cp_t *rt_texture_cp_new_with_colour(colour_t colour_even, colour_t colour_odd)
{
    return rt_texture_cp_new_with_textures((rt_texture_t *)rt_texture_sc_new(colour_even),
                                           (rt_texture_t *)rt_texture_sc_new(colour_odd));
}

rt_texture_cp_t *rt_texture_cp_new_with_textures(rt_texture_t *even, rt_texture_t *odd)
{
    assert(NULL != even);
    assert(NULL != odd);

    rt_texture_cp_t *result = calloc(1, sizeof(rt_texture_cp_t));
    assert(NULL != result);

    result->even = even;
    result->odd = odd;
    rt_texture_init(&result->base, RT_TEXTURE_TYPE_CHECKER);

    return result;
}

colour_t rt_texture_cp_value(const rt_texture_cp_t *texture_cp, double u, double v, const vec3_t *p)
{
    assert(NULL != texture_cp);
    assert(NULL != p);

    double sines = sin(10 * p->x) * sin(10 * p->y) * sin(10 * p->z);
    if (sines > 0)
    {
        return rt_texture_value(texture_cp->even, u, v, p);
    }

    return rt_texture_value(texture_cp->odd, u, v, p);
}

void rt_texture_cp_delete(rt_texture_cp_t *texture_cp)
{
    if (NULL == texture_cp)
    {
        return;
    }
    rt_texture_delete(texture_cp->even);
    rt_texture_delete(texture_cp->odd);
    free(texture_cp);
}