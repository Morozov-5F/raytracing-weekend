/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <rt_material.h>
#include <rt_material_shared.h>
#include <assert.h>

typedef struct rt_material_dl_s
{
    rt_material_t base;

    rt_texture_t *texture;
    double intensity;
} rt_material_dl_t;

static colour_t rt_mt_dl_emit(const rt_material_t *material, double u, double v, const point3_t *p);
static void rt_mt_dl_delete(rt_material_t *material);

rt_material_t *rt_mt_dl_new_with_albedo(colour_t albedo, double intensity)
{
    return rt_mt_dl_new_with_texture(rt_texture_sc_new(albedo), intensity);
}

rt_material_t *rt_mt_dl_new_with_texture(rt_texture_t *texture, double intensity)
{
    rt_material_dl_t *result = calloc(1, sizeof(rt_material_dl_t));
    assert(NULL != result);

    result->texture = texture;
    result->intensity = intensity;
    rt_material_base_init(&result->base, RT_MATERIAL_TYPE_DIFFUSE_LIGHT, NULL, rt_mt_dl_emit, rt_mt_dl_delete);

    return (rt_material_t *)result;
}

static colour_t rt_mt_dl_emit(const rt_material_t *material, double u, double v, const point3_t *p)
{
    assert(NULL != material);
    assert(RT_MATERIAL_TYPE_DIFFUSE_LIGHT == material->type);

    rt_material_dl_t *dl = (rt_material_dl_t *)material;
    return vec3_scale(rt_texture_value(dl->texture, u, v, p), dl->intensity);
}

static void rt_mt_dl_delete(rt_material_t *material)
{
    if (NULL == material)
    {
        return;
    }
    assert(RT_MATERIAL_TYPE_DIFFUSE_LIGHT == material->type);
    rt_material_dl_t *dl = (rt_material_dl_t *)material;

    rt_texture_delete(dl->texture);
    free(dl);
}
