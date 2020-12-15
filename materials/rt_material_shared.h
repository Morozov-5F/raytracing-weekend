/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_SHARED_H

#include "rt_material.h"

typedef enum rt_material_type_e
{
    RT_MATERIAL_TYPE_UNKNOWN = 0,
    RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN = 1,
    RT_MATERIAL_TYPE_METAL = 2,
    RT_MATERIAL_TYPE_DIELECTRIC = 3,
    RT_MATERIAL_TYPE_DIFFUSE_LIGHT = 4,
    RT_MATERIAL_TYPE_ISOTROPIC = 5,
} rt_material_type_t;

typedef bool (*rt_material_scatter_fn)(const rt_material_t *material, const ray_t *incoming_ray,
                                       const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray);

typedef colour_t (*rt_material_emit_fn)(const rt_material_t *material, double u, double v, const point3_t *p);

typedef void (*rt_material_delete_fn)(rt_material_t *material);

struct rt_material_s
{
    rt_material_type_t type;
    int refcount;

    rt_material_scatter_fn scatter;
    rt_material_emit_fn emit;
    rt_material_delete_fn delete;
};

void rt_material_base_init(rt_material_t *material_base, rt_material_type_t type, rt_material_scatter_fn scatter_fn,
                           rt_material_emit_fn emit_fn, rt_material_delete_fn delete_fn);

#endif // RAY_TRACING_ONE_WEEK_RT_MATERIAL_SHARED_H
