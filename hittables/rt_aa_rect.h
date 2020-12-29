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
#include "rt_hittable.h"

typedef enum rt_aa_rect_type_e
{
    RT_AA_RECT_TYPE_XY,
    RT_AA_RECT_TYPE_YZ,
    RT_AA_RECT_TYPE_XZ,
} rt_aa_rect_type_t;

rt_hittable_t *rt_aa_rect_new(rt_aa_rect_type_t type, double axis1_min, double axis1_max, double axis2_min,
                              double axis2_max, double k, rt_material_t *material);

static inline rt_hittable_t *rt_aa_rect_new_xy(double x0, double x1, double y0, double y1, double k,
                                               rt_material_t *material)
{
    return rt_aa_rect_new(RT_AA_RECT_TYPE_XY, x0, x1, y0, y1, k, material);
}

static inline rt_hittable_t *rt_aa_rect_new_yz(double y0, double y1, double z0, double z1, double k,
                                               rt_material_t *material)
{
    return rt_aa_rect_new(RT_AA_RECT_TYPE_YZ, y0, y1, z0, z1, k, material);
}

static inline rt_hittable_t *rt_aa_rect_new_xz(double x0, double x1, double z0, double z1, double k,
                                               rt_material_t *material)
{
    return rt_aa_rect_new(RT_AA_RECT_TYPE_XZ, x0, x1, z0, z1, k, material);
}


#endif // RAY_TRACING_ONE_WEEK_RT_AA_RECT_H
