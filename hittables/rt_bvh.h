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

typedef struct rt_bvh_node_s rt_bvh_node_t;

rt_bvh_node_t *rt_bvh_node_new(const rt_hittable_list_t *hittable_list, double time0, double time1);

void rt_bvh_node_delete(rt_bvh_node_t *bvh_node);

bool rt_bvh_node_hit(const rt_bvh_node_t *bvh_node, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

bool rt_bvh_node_bb(const rt_bvh_node_t *bvh_node, double time0, double time1, rt_aabb_t *out_bb);

#endif // RAY_TRACING_ONE_WEEK_RT_BVH_H
