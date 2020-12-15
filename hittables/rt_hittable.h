/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_H

#include <rt_weekend.h>
#include <stdbool.h>

#include <rt_hit.h>
#include <rt_aabb.h>
#include <rt_material.h>

typedef struct rt_hittable_s rt_hittable_t;

rt_hittable_t *rt_hittable_claim(rt_hittable_t *hittable);

bool rt_hittable_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record);

void rt_hittable_delete(rt_hittable_t *hittable);

bool rt_hittable_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);

// Constant medium
rt_hittable_t *rt_const_medium_new_with_texture(rt_hittable_t *boundary, double density, rt_texture_t *texture);
rt_hittable_t *rt_const_medium_new_with_colour(rt_hittable_t *boundary, double density, colour_t colour);

// Box
rt_hittable_t *rt_box_new(point3_t min, point3_t max, rt_material_t *material);

// Sphere
rt_hittable_t *rt_sphere_new(point3_t center, double radius, rt_material_t *material);

// Moving sphere
rt_hittable_t *rt_moving_sphere_new(point3_t center_start, point3_t center_end, double time_start, double time_end,
                                    double radius, rt_material_t *material);

#endif // RAY_TRACING_ONE_WEEK_RT_HITTABLE_H
