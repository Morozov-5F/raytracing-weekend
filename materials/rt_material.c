/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_material.h"
#include "rt_material_shared.h"

static bool scatter_base(const rt_material_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                         colour_t *attenuation, ray_t *scattered_ray);
static colour_t emit_base(const rt_material_t *material, double u, double v, const point3_t *p);

static void delete_base(rt_material_t *material);

void rt_material_base_init(rt_material_t *material_base, rt_material_type_t type, rt_material_scatter_fn scatter_fn,
                           rt_material_emit_fn emit_fn, rt_material_delete_fn delete_fn)
{
    assert(NULL != material_base);
    material_base->type = type;
    material_base->refcount = 1;

    if (NULL == scatter_fn)
    {
        scatter_fn = scatter_base;
    }
    material_base->scatter = scatter_fn;

    if (NULL == emit_fn)
    {
        emit_fn = emit_base;
    }
    material_base->emit = emit_fn;

    if (NULL == delete_fn)
    {
        delete_fn = delete_base;
    }
    material_base->delete = delete_fn;
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

    return material->scatter(material, incoming_ray, hit_record, attenuation, scattered_ray);
}

colour_t rt_material_emit(const rt_material_t *material, double u, double v, const point3_t *p)
{
    assert(NULL != material);

    return material->emit(material, u, v, p);
}

void rt_material_delete(rt_material_t *material)
{
    if (NULL == material || --material->refcount > 0)
    {
        return;
    }

    material->delete (material);
}

static bool scatter_base(const rt_material_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                         colour_t *attenuation, ray_t *scattered_ray)
{
    return false;
}

static colour_t emit_base(const rt_material_t *material, double u, double v, const point3_t *p)
{
    return colour(0.0, 0.0, 0.0);
}

static void delete_base(rt_material_t *material)
{
    free(material);
}
