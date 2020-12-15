/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_texture_shared.h>
#include <assert.h>

typedef struct rt_texture_cp_s
{
    rt_texture_t base;

    rt_texture_t *even;
    rt_texture_t *odd;
} rt_texture_cp_t;

static colour_t rt_texture_cp_value(const rt_texture_t *texture, double u, double v, const vec3_t *p);
static void rt_texture_cp_delete(rt_texture_t *texture);

rt_texture_t *rt_texture_cp_new_with_colour(colour_t colour_even, colour_t colour_odd)
{
    return rt_texture_cp_new_with_textures(rt_texture_sc_new(colour_even), rt_texture_sc_new(colour_odd));
}

rt_texture_t *rt_texture_cp_new_with_textures(rt_texture_t *even, rt_texture_t *odd)
{
    assert(NULL != even);
    assert(NULL != odd);

    rt_texture_cp_t *result = calloc(1, sizeof(rt_texture_cp_t));
    assert(NULL != result);

    result->even = even;
    result->odd = odd;
    rt_texture_init(&result->base, RT_TEXTURE_TYPE_CHECKER, rt_texture_cp_value, rt_texture_cp_delete);

    return (rt_texture_t *)result;
}

static colour_t rt_texture_cp_value(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    assert(NULL != texture);
    assert(NULL != p);
    assert(RT_TEXTURE_TYPE_CHECKER == texture->type);

    rt_texture_cp_t *checker = (rt_texture_cp_t *)texture;

    double sines = sin(10 * p->x) * sin(10 * p->y) * sin(10 * p->z);
    if (sines > 0)
    {
        return rt_texture_value(checker->even, u, v, p);
    }

    return rt_texture_value(checker->odd, u, v, p);
}

static void rt_texture_cp_delete(rt_texture_t *texture)
{
    if (NULL == texture)
    {
        return;
    }

    assert(RT_TEXTURE_TYPE_CHECKER == texture->type);

    rt_texture_cp_t *checker = (rt_texture_cp_t *)texture;

    rt_texture_delete(checker->even);
    rt_texture_delete(checker->odd);

    free(checker);
}