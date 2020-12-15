/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_BVH_H
#define RAY_TRACING_ONE_WEEK_RT_BVH_H

#include <rt_weekend.h>
#include <rt_hittable_list.h>
#include <stdbool.h>

rt_hittable_t *rt_bvh_node_new(const rt_hittable_list_t *hittable_list, double time0, double time1);

#endif // RAY_TRACING_ONE_WEEK_RT_BVH_H
