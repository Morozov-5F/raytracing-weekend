/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_material_diffuse_light.h"
#include <rt_material_shared.h>
#include <rt_texture_solid_colour.h>
#include <assert.h>

struct rt_material_dl_s
{
    rt_material_t base;

    rt_texture_t *texture;
};

rt_material_dl_t *rt_mt_dl_new_with_albedo(colour_t albedo)
{
    return rt_mt_dl_new_with_texture((rt_texture_t *)rt_texture_sc_new(albedo));
}

rt_material_dl_t *rt_mt_dl_new_with_texture(rt_texture_t *texture)
{
    rt_material_dl_t *result = calloc(1, sizeof(rt_material_dl_t));
    assert(NULL != result);

    result->texture = texture;
    rt_material_base_init(&result->base, RT_MATERIAL_TYPE_DIFFUSE_LIGHT);

    return result;
}

bool rt_mt_dl_scatter(const rt_material_dl_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                      colour_t *attenuation, ray_t *scattered_ray)
{
    return false;
}

colour_t rt_mt_dl_emit(const rt_material_dl_t *material, double u, double v, const point3_t *p)
{
    assert(NULL != material);
    return rt_texture_value(material->texture, u, v, p);
}

void rt_mt_dl_delete(rt_material_dl_t *material)
{
    if (NULL == material)
    {
        return;
    }
    rt_texture_delete(material->texture);
    free(material);
}
