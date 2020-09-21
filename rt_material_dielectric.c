/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_material_dielectric.h"
#include "rt_material_shared.h"

struct rt_material_dielectric_s
{
    rt_material_t base;

    double refraction_factor;
};

static double schlick_approximation(double cosine, double refraction_factor)
{
    double r0 = (1.0 - refraction_factor) / (1.0 + refraction_factor);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

rt_material_dielectric_t *rt_mt_dielectric_new(double refraction_factor)
{
    rt_material_dielectric_t *material = calloc(1, sizeof(rt_material_dielectric_t));
    assert(NULL != material);

    rt_material_base_init(&material->base, RT_MATERIAL_TYPE_DIELECTRIC);
    material->refraction_factor = refraction_factor;

    return material;
}

bool rt_mt_dielectric_scatter(const rt_material_dielectric_t *material, const ray_t *incoming_ray,
                              const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray)
{
    assert(NULL != material);
    assert(NULL != incoming_ray);
    assert(NULL != hit_record);
    assert(NULL != attenuation);
    assert(NULL != scattered_ray);

    *attenuation = colour(1.0, 1.0, 1.0);
    double r = hit_record->front_face ? (1.0 / material->refraction_factor) : material->refraction_factor;

    vec3_t direction_unit = vec3_normalized(incoming_ray->direction);
    double cos_theta = fmin(vec3_dot(vec3_negate(&direction_unit), hit_record->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (r * sin_theta > 1.0 || rt_random_double(0, 1) < schlick_approximation(cos_theta, r))
    {
        vec3_t reflected = vec3_reflect(&direction_unit, &hit_record->normal);
        *scattered_ray = ray_init(hit_record->p, reflected);
        return true;
    }
    vec3_t refracted = vec3_refract(&direction_unit, &hit_record->normal, r);
    *scattered_ray = ray_init(hit_record->p, refracted);

    return true;
}

void rt_mt_dielectric_delete(rt_material_dielectric_t *dielectric)
{
    free(dielectric);
}
