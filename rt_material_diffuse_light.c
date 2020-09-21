/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_material_diffuse_light.h"
#include "rt_material.h"
#include "rt_material_shared.h"

struct rt_material_diffuse_light_s
{
    rt_material_t base;

    colour_t albedo;
};

bool rt_mt_diffuse_light_scatter(const rt_material_diffuse_light_t *material, const ray_t *incoming_ray,
                                 const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray)
{
    return false;
}

colour_t rt_mt_diffuse_light_emitted(const rt_material_diffuse_light_t *material)
{
    assert(NULL != material);
    return material->albedo;
}

rt_material_diffuse_light_t *rt_mt_diffuse_light_new(colour_t albedo)
{
    rt_material_diffuse_light_t * material = calloc(1, sizeof(rt_material_diffuse_light_t));
    assert(NULL != material);

    rt_material_base_init(&material->base, RT_MATERIAL_TYPE_DIFFUSE_LIGHT);
    material->albedo = albedo;

    return material;
}

void rt_mt_diffuse_light_delete(rt_material_diffuse_light_t *diffuse_light)
{
    free(diffuse_light);
}
