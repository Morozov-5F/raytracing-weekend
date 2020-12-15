/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_material.h"
#include "rt_material_shared.h"

typedef struct rt_material_diffuse_s
{
    rt_material_t base;

    rt_texture_t *texture;
} rt_material_diffuse_t;

static bool rt_mt_diffuse_scatter(const rt_material_t *material, const ray_t *incoming_ray,
                                  const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray);
static void rt_mt_diffuse_delete(rt_material_t *material);

rt_material_t *rt_mt_diffuse_new_with_albedo(colour_t albedo)
{
    return rt_mt_diffuse_new_with_texture(rt_texture_sc_new_with_components(albedo.x, albedo.y, albedo.z));
}

rt_material_t *rt_mt_diffuse_new_with_texture(rt_texture_t *texture)
{
    rt_material_diffuse_t *material = calloc(1, sizeof(rt_material_diffuse_t));
    assert(NULL != material);

    material->texture = texture;
    rt_material_base_init(&material->base, RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN, rt_mt_diffuse_scatter, NULL,
                          rt_mt_diffuse_delete);
    return (rt_material_t *)material;
}

static bool rt_mt_diffuse_scatter(const rt_material_t *material, const ray_t *incoming_ray,
                                  const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray)
{
    assert(NULL != material);
    assert(NULL != incoming_ray);
    assert(NULL != hit_record);
    assert(NULL != attenuation);
    assert(NULL != scattered_ray);

    assert(RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN == material->type);
    (void)incoming_ray;

    rt_material_diffuse_t *diffuse = (rt_material_diffuse_t *)material;

    vec3_t scatter_direction = vec3_sum(hit_record->normal, vec3_random_unit_vector());
    *scattered_ray = ray_init(hit_record->p, scatter_direction, incoming_ray->time);
    *attenuation = rt_texture_value(diffuse->texture, hit_record->u, hit_record->v, &hit_record->p);

    return true;
}

static void rt_mt_diffuse_delete(rt_material_t *material)
{
    if (NULL == material)
    {
        return;
    }

    assert(RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN == material->type);
    rt_material_diffuse_t *diffuse = (rt_material_diffuse_t *)material;

    rt_texture_delete(diffuse->texture);
    free(diffuse);
}
