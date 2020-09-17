//
// Created by morozov on 17.09.2020.
//

#include <assert.h>
#include "rt_material_dielectric.h"
#include "rt_material_shared.h"

struct rt_material_dielectric_s
{
    rt_material_t base;

    double refraction_factor;
};

rt_material_dielectric_t *rt_mt_dielectric_new(double refraction_factor)
{
    rt_material_dielectric_t *material = calloc(1, sizeof(rt_material_dielectric_t));
    assert(NULL != material);

    material->base.type = RT_MATERIAL_TYPE_DIELECTRIC;
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

    *attenuation = colour3(1.0, 1.0, 1.0);
    double r = hit_record->front_face ? (1.0 / material->refraction_factor) : material->refraction_factor;

    vec3_t refracted = vec3_refract(&incoming_ray->direction, &hit_record->normal, r);
    *scattered_ray = ray_init(hit_record->p, refracted);

    return true;
}

void rt_mt_dielectric_delete(rt_material_dielectric_t *dielectric)
{
    free(dielectric);
}
