/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_CONST_MEDIUM_H
#define RAY_TRACING_ONE_WEEK_RT_CONST_MEDIUM_H

#include <rt_material.h>
#include <rt_texture.h>
#include "rt_hittable.h"

typedef struct rt_const_medium_s rt_const_medium_t;

rt_const_medium_t *rt_const_medium_new_with_texture(rt_hittable_t *hittable, double density, rt_texture_t *texture);
rt_const_medium_t *rt_const_medium_new_with_colour(rt_hittable_t *hittable, double density, colour_t colour);

void rt_const_medium_delete(rt_const_medium_t *medium);

bool rt_const_medium_hit(const rt_const_medium_t *medium, const ray_t *ray, double t_min, double t_max,
                          rt_hit_record_t *record);

bool rt_const_medium_bb(const rt_const_medium_t *medium, double time0, double time1, rt_aabb_t *out_bb);

#endif // RAY_TRACING_ONE_WEEK_RT_CONST_MEDIUM_H
