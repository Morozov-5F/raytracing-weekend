/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_SCENES_H
#define RAY_TRACING_ONE_WEEK_RT_SCENES_H

#include <rt_hittable_list.h>

typedef enum rt_scene_id_s
{
    RT_SCENE_RANDOM,
    RT_SCENE_TWO_SPHERES,
    RT_SCENE_TWO_PERLIN_SPHERES,
} rt_scene_id_t;

rt_hittable_list_t *rt_scene_random(void);

rt_hittable_list_t *rt_scene_two_spheres(void);

rt_hittable_list_t *rt_scene_two_perlin_spheres(void);

#endif // RAY_TRACING_ONE_WEEK_RT_SCENES_H
