//
// Created by morozov on 16.09.2020.
//

#include <assert.h>
#include "rt_material_diffuse.h"
#include "rt_material_shared.h"

struct rt_material_diffuse_s
{
    rt_material_t base;

    colour_t albedo;
};

rt_material_diffuse_t *rt_mt_diffuse_new(colour_t albedo)
{
    rt_material_diffuse_t * material = calloc(1, sizeof(rt_material_diffuse_t));
    assert(NULL != material);

    material->base.type = RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN;
    material->albedo = albedo;

    return material;
}

bool
rt_mt_diffuse_scatter(const rt_material_diffuse_t *material, const rt_hit_record_t *hit_record, colour_t *attenuation,
                      ray_t *scattered_ray)
{
    assert(NULL != material);
    assert(NULL != hit_record);
    assert(NULL != attenuation);
    assert(NULL != scattered_ray);

    vec3_t scatter_direction = vec3_sum(hit_record->normal, vec3_random_unit_vector());
    *scattered_ray = ray_init(hit_record->p, scatter_direction);
    *attenuation = material->albedo;

    return true;
}

void rt_mt_diffuse_delete(rt_material_diffuse_t *diffuse)
{
    free(diffuse);
}
