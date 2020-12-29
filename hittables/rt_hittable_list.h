/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_LIST_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_LIST_H

#include "../rt_weekend.h"
#include "rt_hittable.h"
#include <stdlib.h>

typedef struct rt_hittable_list_s rt_hittable_list_t;

rt_hittable_list_t *rt_hittable_list_init(size_t capacity);

void rt_hittable_list_add(rt_hittable_list_t *list, rt_hittable_t *hittable);
void rt_hittable_list_clear(rt_hittable_list_t *list);
void rt_hittable_list_deinit(rt_hittable_list_t *list);

bool rt_hittable_list_hit_test(const rt_hittable_list_t *list, const ray_t *ray, double t_min, double t_max,
                               rt_hit_record_t *record);

bool rt_hittable_list_bb(const rt_hittable_list_t *list, double time0, double time1, rt_aabb_t *out_bb);

size_t rt_hittable_list_get_size(const rt_hittable_list_t *list);

rt_hittable_t **rt_hittable_list_get_underlying_container(const rt_hittable_list_t *list);

#endif // RAY_TRACING_ONE_WEEK_RT_HITTABLE_LIST_H
