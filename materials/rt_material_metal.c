/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_material.h"
#include "rt_material_shared.h"

typedef struct rt_material_metal_s
{
    rt_material_t base;

    colour_t albedo;
    double fuzziness;
} rt_material_metal_t;

static bool rt_mt_metal_scatter(const rt_material_t *material, const ray_t *incoming_ray,
                                const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray);

rt_material_t *rt_mt_metal_new(colour_t albedo, double fuzziness)
{
    rt_material_metal_t *material = calloc(1, sizeof(rt_material_metal_t));
    assert(NULL != material);

    rt_material_base_init(&material->base, RT_MATERIAL_TYPE_METAL, rt_mt_metal_scatter, NULL, NULL);

    material->albedo = albedo;
    material->fuzziness = fuzziness > 1 ? 1 : fuzziness;

    return (rt_material_t *)material;
}

static bool rt_mt_metal_scatter(const rt_material_t *material, const ray_t *incoming_ray,
                                const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray)
{
    assert(NULL != material);
    assert(NULL != incoming_ray);
    assert(NULL != hit_record);
    assert(NULL != attenuation);
    assert(NULL != scattered_ray);

    assert(RT_MATERIAL_TYPE_METAL == material->type);

    rt_material_metal_t *metal = (rt_material_metal_t *)material;

    vec3_t direction_normalized = vec3_normalized(incoming_ray->direction);
    vec3_t reflected = vec3_reflect(&direction_normalized, &hit_record->normal);
    if (metal->fuzziness > 0)
    {
        vec3_add(&reflected, vec3_scale(vec3_random_in_unit_sphere(), metal->fuzziness));
    }
    *scattered_ray = ray_init(hit_record->p, reflected, incoming_ray->time);
    *attenuation = metal->albedo;

    return vec3_dot(scattered_ray->direction, hit_record->normal) > 0;
}
