/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_AA_RECT_H
#define RAY_TRACING_ONE_WEEK_RT_AA_RECT_H

#include <rt_weekend.h>
#include <rt_material.h>
#include <rt_aabb.h>

typedef enum rt_aa_rect_type_e
{
    RT_AA_RECT_TYPE_XY,
    RT_AA_RECT_TYPE_YZ,
    RT_AA_RECT_TYPE_XZ,
} rt_aa_rect_type_t;

typedef struct rt_aa_rect_s rt_aa_rect_t;

rt_aa_rect_t *rt_aa_rect_new(rt_aa_rect_type_t type, point3_t top_left, double width, double height, rt_material_t *material);

static inline rt_aa_rect_t *rt_aa_rect_new_xy(point3_t top_left, double width, double height, rt_material_t *material)
{
    return rt_aa_rect_new(RT_AA_RECT_TYPE_XY, top_left, width, height, material);
}

static inline rt_aa_rect_t *rt_aa_rect_new_yz(point3_t top_left, double width, double height, rt_material_t *material)
{
    return rt_aa_rect_new(RT_AA_RECT_TYPE_YZ, top_left, width, height, material);
}

static inline rt_aa_rect_t *rt_aa_rect_new_xz(point3_t top_left, double width, double height, rt_material_t *material)
{
    return rt_aa_rect_new(RT_AA_RECT_TYPE_XZ, top_left, width, height, material);
}

void rt_aa_rect_delete(rt_aa_rect_t *rect);

bool rt_aa_rect_hit(const rt_aa_rect_t *rect, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

bool rt_aa_rect_bb(const rt_aa_rect_t *rect, double time0, double time1, rt_aabb_t *out_bb);

#endif // RAY_TRACING_ONE_WEEK_RT_AA_RECT_H
