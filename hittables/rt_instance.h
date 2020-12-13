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

typedef struct rt_instance_s rt_instance_t;

rt_instance_t *rt_instance_new(rt_hittable_t *hittable);

void rt_instance_translate(rt_instance_t *instance, point3_t offset);

void rt_instance_rotate_x(rt_instance_t *instance, double x);

void rt_instance_rotate_y(rt_instance_t *instance, double y);

void rt_instance_rotate_z(rt_instance_t *instance, double z);

void rt_instance_delete(rt_instance_t *instance);

bool rt_instance_hit(const rt_instance_t *instance, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

bool rt_instance_bb(const rt_instance_t *instance, double time0, double time1, rt_aabb_t *out_bb);


#endif // RAY_TRACING_ONE_WEEK_RT_INSTANCE_H
