/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_material_isotropic.h"
#include "rt_material_shared.h"

struct rt_material_iso_s
{
    rt_material_t base;

    rt_texture_t *albedo;
};

rt_material_iso_t *rt_mt_iso_new_with_albedo(colour_t albedo)
{
    return rt_mt_iso_new_with_texture(rt_texture_sc_new(albedo));
}

rt_material_iso_t *rt_mt_iso_new_with_texture(rt_texture_t *texture)
{
    rt_material_iso_t *result = calloc(1, sizeof(rt_material_iso_t));
    assert(NULL != result);

    result->albedo = texture;

    rt_material_base_init(&result->base, RT_MATERUAL_TYPE_ISOTROPIC);

    return result;
}

bool rt_mt_iso_scatter(const rt_material_iso_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                       colour_t *attenuation, ray_t *scattered_ray)
{
    assert(NULL != material);

    *scattered_ray = ray_init(hit_record->p, vec3_random_in_unit_sphere(), incoming_ray->time);
    *attenuation = rt_texture_value(material->albedo, hit_record->u, hit_record->v, &hit_record->p);

    return true;
}

void rt_mt_iso_delete(rt_material_iso_t *material)
{
    if (NULL == material)
    {
        return;
    }

    rt_texture_delete(material->albedo);
    free(material);
}
