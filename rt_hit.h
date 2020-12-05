/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_HIT_H
#define RAY_TRACING_ONE_WEEK_RT_HIT_H

#include "rt_weekend.h"
#include <stdbool.h>

typedef struct rt_hit_record_s
{
    point3_t p;
    vec3_t normal;
    void *material;

    double t;

    double u;
    double v;

    bool front_face;
} rt_hit_record_t;

void rt_hit_record_set_front_face(rt_hit_record_t *record, const ray_t *ray, const vec3_t *outward_normal);

#endif // RAY_TRACING_ONE_WEEK_RT_HIT_H
