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

typedef struct rt_hittable_s rt_hittable_t;

bool rt_hittable_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                     rt_hit_record_t *record);

rt_hittable_t *rt_hittable_claim(rt_hittable_t *hittable);

void rt_hittable_delete(rt_hittable_t *hittable);

bool rt_hittable_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);

#endif // RAY_TRACING_ONE_WEEK_RT_HITTABLE_H
