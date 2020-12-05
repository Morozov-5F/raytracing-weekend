/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_SPHERE_H
#define RAY_TRACING_ONE_WEEK_RT_SPHERE_H

#include <stdbool.h>
#include <rt_material.h>

#include "rt_hittable.h"

typedef struct rt_sphere_s rt_sphere_t;

rt_sphere_t *rt_sphere_new(point3_t center, double radius, rt_material_t *material);
void rt_sphere_delete(rt_sphere_t *sphere);

bool rt_sphere_hit(const rt_sphere_t *sphere, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

bool rt_sphere_hit_test_generic(point3_t center, double radius, rt_material_t *material, const ray_t *ray, double t_min,
                                double t_max, rt_hit_record_t *record);

bool rt_sphere_bb(const rt_sphere_t *sphere, double time0, double time1, rt_aabb_t *out_bb);

#endif // RAY_TRACING_ONE_WEEK_RT_SPHERE_H