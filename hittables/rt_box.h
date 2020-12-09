/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_BOX_H
#define RAY_TRACING_ONE_WEEK_RT_BOX_H

#include <rt_weekend.h>
#include <rt_material.h>
#include <rt_aabb.h>

typedef struct rt_box_s rt_box_t;

rt_box_t *rt_box_new(point3_t min, point3_t max, rt_material_t *material);

void rt_box_delete(rt_box_t *box);

bool rt_box_hit(const rt_box_t *box, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

bool rt_box_bb(const rt_box_t *box, double time0, double time1, rt_aabb_t *out_bb);

#endif // RAY_TRACING_ONE_WEEK_RT_BOX_H
