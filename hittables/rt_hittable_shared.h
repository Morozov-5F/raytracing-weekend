/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H

#include <stdbool.h>
#include <rt_material.h>
#include "rt_hittable.h"

typedef enum rt_hittable_type_e
{
    RT_HITTABLE_TYPE_UNKNOWN = -1,
    RT_HITTABLE_TYPE_SPHERE = 0,
    RT_HITTABLE_TYPE_MOVING_SPHERE,
    RT_HITTABLE_TYPE_BVH_NODE,
    RT_HITTABLE_TYPE_AA_RECT,
    RT_HITTABLE_TYPE_BOX,
} rt_hittable_type_t;

struct rt_hittable_s
{
    rt_hittable_type_t type;
    int refcount;
};

bool rt_sphere_hit_test_generic(point3_t center, double radius, rt_material_t *material, const ray_t *ray, double t_min,
                                double t_max, rt_hit_record_t *record);


void rt_hittable_init(rt_hittable_t *hittable, rt_hittable_type_t type);

typedef int(*rt_hittable_compare_fn)(const void *a, const void *b);

int rt_hittable_box_cmp_x(const void *a, const void *b);

int rt_hittable_box_cmp_y(const void *a, const void *b);

int rt_hittable_box_cmp_z(const void *a, const void *b);

void rt_get_sphere_uv(const point3_t *p, double *u, double *v);

#endif // RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
