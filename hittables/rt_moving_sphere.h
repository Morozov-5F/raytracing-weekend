/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_MOVING_SPHERE_H
#define RAY_TRACING_ONE_WEEK_RT_MOVING_SPHERE_H

#include <rt_material.h>
#include "rt_hittable.h"

typedef struct rt_moving_sphere_s rt_moving_sphere_t;

rt_moving_sphere_t *rt_moving_sphere_new(point3_t center_start, point3_t center_end, double time_start, double time_end,
                                         double radius, rt_material_t *material);
void rt_moving_sphere_delete(rt_moving_sphere_t *moving_sphere);

bool rt_moving_sphere_hit(const rt_moving_sphere_t *moving_sphere, const ray_t *ray, double t_min, double t_max,
                          rt_hit_record_t *record);

#endif // RAY_TRACING_ONE_WEEK_RT_MOVING_SPHERE_H
