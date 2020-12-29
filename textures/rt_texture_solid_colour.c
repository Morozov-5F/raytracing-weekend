/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_texture_shared.h>
#include <assert.h>
#include "rt_texture.h"

typedef struct rt_texture_sc_s
{
    rt_texture_t base;

    colour_t colour;
} rt_texture_sc_t;

static colour_t rt_texture_sc_value(const rt_texture_t *texture, double u, double v, const vec3_t *p);
static void rt_texture_sc_delete(rt_texture_t *texture);

rt_texture_t *rt_texture_sc_new(colour_t colour)
{
    rt_texture_sc_t *result = calloc(1, sizeof(rt_texture_sc_t));
    assert(NULL != result);

    result->colour = colour;
    rt_texture_init(&result->base, RT_TEXTURE_TYPE_SOLID_COLOUR, rt_texture_sc_value, rt_texture_sc_delete);

    return (rt_texture_t *)result;
}

rt_texture_t *rt_texture_sc_new_with_components(double red, double green, double blue)
{
    return rt_texture_sc_new(colour(red, green, blue));
}

static colour_t rt_texture_sc_value(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    assert(NULL != texture);
    assert(RT_TEXTURE_TYPE_SOLID_COLOUR == texture->type);

    return ((rt_texture_sc_t *)texture)->colour;
}

static void rt_texture_sc_delete(rt_texture_t *texture)
{
    if (NULL == texture)
    {
        return;
    }
    assert(RT_TEXTURE_TYPE_SOLID_COLOUR == texture->type);

    free(texture);
}