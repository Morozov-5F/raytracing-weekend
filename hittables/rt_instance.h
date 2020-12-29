/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_INSTANCE_H
#define RAY_TRACING_ONE_WEEK_RT_INSTANCE_H

#include <rt_aabb.h>
#include <rt_weekend.h>
#include <rt_hittable.h>

rt_hittable_t *rt_instance_new(rt_hittable_t *hittable);

void rt_instance_translate(rt_hittable_t *instance, point3_t offset);

void rt_instance_rotate_y(rt_hittable_t *instance, double y);

#endif // RAY_TRACING_ONE_WEEK_RT_INSTANCE_H
