/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include <rt_perlin.h>
#include <rt_texture_shared.h>

#include "rt_texture_noise.h"

struct rt_texture_noise_s
{
    rt_texture_t base;
    rt_perlin_t *perlin;

    int intensity;
};

rt_texture_noise_t *rt_texture_noise_new(int intensity)
{
    rt_texture_noise_t *result = calloc(1, sizeof(rt_texture_noise_t));
    assert(NULL != result);

    result->perlin = rt_perlin_new();
    assert(NULL != result->perlin);
    result->intensity = intensity;

    rt_texture_init(&result->base, RT_TEXTURE_TYPE_NOISE);

    return result;
}

colour_t rt_texture_noise_value(const rt_texture_noise_t *texture_noise, double u, double v, const vec3_t *p)
{
    assert(NULL != texture_noise);
    assert(NULL != p);

    colour_t base = colour(1.0, 1.0, 1.0);
    return vec3_scale(base, rt_perlin_noise(texture_noise->perlin, vec3_scale(*p, texture_noise->intensity)));
    //    return vec3_scale(base, rt_perlin_blocky_noise(texture_noise->perlin, *p));
}

void rt_texture_noise_delete(rt_texture_noise_t *texture_noise)
{
    if (NULL == texture_noise)
    {
        return;
    }
    rt_perlin_delete(texture_noise->perlin);
    free(texture_noise);
}
