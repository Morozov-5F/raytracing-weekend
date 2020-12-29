/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include <rt_perlin.h>
#include <rt_texture_shared.h>

typedef struct rt_texture_noise_s
{
    rt_texture_t base;
    rt_perlin_t *perlin;

    double intensity;
} rt_texture_noise_t;

static colour_t rt_texture_noise_value(const rt_texture_t *texture, double u, double v, const vec3_t *p);
static void rt_texture_noise_delete(rt_texture_t *texture);

rt_texture_t *rt_texture_noise_new(double intensity)
{
    rt_texture_noise_t *result = calloc(1, sizeof(rt_texture_noise_t));
    assert(NULL != result);

    result->perlin = rt_perlin_new();
    assert(NULL != result->perlin);
    result->intensity = intensity;

    rt_texture_init(&result->base, RT_TEXTURE_TYPE_NOISE, rt_texture_noise_value, rt_texture_noise_delete);

    return (rt_texture_t *)result;
}

colour_t rt_texture_noise_value(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    assert(NULL != texture);
    assert(NULL != p);
    assert(RT_TEXTURE_TYPE_NOISE == texture->type);

    const rt_texture_noise_t *noise = (rt_texture_noise_t *)texture;

    colour_t base = colour(1.0, 1.0, 1.0);

    double turbulence = rt_perlin_turbulence(noise->perlin, *p, 7);
    return vec3_scale(base, 0.5 * (1.0 + sin(noise->intensity * p->z + 10 * turbulence)));
}

void rt_texture_noise_delete(rt_texture_t *texture)
{
    if (NULL == texture)
    {
        return;
    }
    assert(RT_TEXTURE_TYPE_NOISE == texture->type);

    const rt_texture_noise_t *noise = (rt_texture_noise_t *)texture;

    rt_perlin_delete(noise->perlin);
    free(texture);
}
