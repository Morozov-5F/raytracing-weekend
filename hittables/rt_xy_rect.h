/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_XY_RECT_H
#define RAY_TRACING_ONE_WEEK_RT_XY_RECT_H

#include <rt_weekend.h>
#include <rt_material.h>
#include <rt_aabb.h>

typedef struct rt_xy_rect_s rt_xy_rect_t;

rt_xy_rect_t *rt_xy_rect_new(point3_t top_left, double width, double height, rt_material_t *material);

void rt_xy_rect_delete(rt_xy_rect_t *xy_rect);

bool rt_xy_rect_hit(const rt_xy_rect_t *xy_rect, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

bool rt_xy_rect_bb(const rt_xy_rect_t *xy_rect, double time0, double time1, rt_aabb_t *out_bb);

#endif // RAY_TRACING_ONE_WEEK_RT_XY_RECT_H
