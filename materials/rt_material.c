/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_material.h"
#include "rt_material_shared.h"
#include "rt_material_diffuse.h"
#include "rt_material_metal.h"
#include "rt_material_dielectric.h"
#include "rt_material_diffuse_light.h"

void rt_material_base_init(rt_material_t *material_base, rt_material_type_t type)
{
    assert(NULL != material_base);
    material_base->type = type;
    material_base->refcount = 1;
}

rt_material_t *rt_material_claim(rt_material_t *material)
{
    assert(NULL != material);
    material->refcount++;

    return material;
}

bool rt_material_scatter(const rt_material_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                         colour_t *attenuation, ray_t *scattered_ray)
{
    assert(NULL != material);
    assert(NULL != hit_record);
    assert(NULL != attenuation);
    assert(NULL != scattered_ray);

    switch (material->type)
    {
        case RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN:
            return rt_mt_diffuse_scatter((rt_material_diffuse_t *)material, incoming_ray, hit_record, attenuation,
                                         scattered_ray);
        case RT_MATERIAL_TYPE_METAL:
            return rt_mt_metal_scatter((rt_material_metal_t *)material, incoming_ray, hit_record, attenuation,
                                       scattered_ray);

        case RT_MATERIAL_TYPE_DIELECTRIC:
            return rt_mt_dielectric_scatter((rt_material_dielectric_t *)material, incoming_ray, hit_record, attenuation,
                                            scattered_ray);

        case RT_MATERIAL_TYPE_DIFFUSE_LIGHT:
            return rt_mt_dl_scatter((rt_material_dl_t *)material, incoming_ray, hit_record, attenuation,
                                    scattered_ray);
        default:
            assert(0);
    }

    return false;
}

colour_t rt_material_emit(const rt_material_t *material, double u, double v, const point3_t *p)
{
    assert(NULL != material);

    switch (material->type)
    {
        case RT_MATERIAL_TYPE_DIFFUSE_LIGHT:
            return rt_mt_dl_emit((rt_material_dl_t *)material, u, v, p);

        default:
            break;
    }

    return colour(0, 0, 0);
}

void rt_material_delete(rt_material_t *material)
{
    if (NULL == material || --(material->refcount) > 0)
    {
        return;
    }

    switch (material->type)
    {
        case RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN:
            rt_mt_diffuse_delete((rt_material_diffuse_t *)material);
            break;
        case RT_MATERIAL_TYPE_METAL:
            rt_mt_metal_delete((rt_material_metal_t *)material);
            break;
        case RT_MATERIAL_TYPE_DIELECTRIC:
            rt_mt_dielectric_delete((rt_material_dielectric_t *)material);
            break;

        case RT_MATERIAL_TYPE_DIFFUSE_LIGHT:
            rt_mt_dl_delete((rt_material_dl_t *)material);
            break;

        default:
            assert(0);
    }
}
