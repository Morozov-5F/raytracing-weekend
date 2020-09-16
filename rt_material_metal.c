//
// Created by morozov on 16.09.2020.
//

#include <assert.h>
#include "rt_material_metal.h"
#include "rt_material_shared.h"

struct rt_material_metal_s
{
    rt_material_t base;

    colour_t albedo;
};


rt_material_metal_t *rt_mt_metal_new(colour_t albedo)
{
    rt_material_metal_t * material = calloc(1, sizeof(rt_material_metal_t));
    assert(NULL != material);

    material->base.type = RT_MATERIAL_TYPE_METAL;
    material->albedo = albedo;

    return material;
}

bool rt_mt_metal_scatter(const rt_material_metal_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                         colour_t *attenuation, ray_t *scattered_ray)
{
    vec3_t reflected = vec3_reflect(&incoming_ray->direction, &hit_record->normal);
    *scattered_ray = ray_init(hit_record->p, reflected);
    *attenuation = material->albedo;

    return vec3_dot(scattered_ray->direction, hit_record->normal);
}

void rt_mt_metal_delete(rt_material_metal_t *metal)
{
    free(metal);
}
