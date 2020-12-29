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
    RT_SCENE_NONE = -1,
    RT_SCENE_RANDOM,
    RT_SCENE_TWO_SPHERES,
    RT_SCENE_TWO_PERLIN_SPHERES,
    RT_SCENE_EARTH,
    RT_SCENE_LIGHT_SAMPLE,
    RT_SCENE_CORNELL_BOX,
    RT_SCENE_INSTANCE_TEST,
    RT_SCENE_CORNELL_SMOKE,
    RT_SCENE_SHOWCASE,
    RT_SCENE_METAL_TEST,
} rt_scene_id_t;

rt_scene_id_t rt_scene_get_id_by_name(const char *name);

const char *rt_scene_get_name_by_id(rt_scene_id_t scene_id);

const char *rt_scene_get_available_scene_names_printable(void);

void rt_scene_print_scenes_info(FILE *to);

rt_hittable_list_t *rt_scene_random(void);

rt_hittable_list_t *rt_scene_two_spheres(void);

rt_hittable_list_t *rt_scene_two_perlin_spheres(void);

rt_hittable_list_t *rt_scene_earth(void);

rt_hittable_list_t *rt_scene_light_sample(void);

rt_hittable_list_t *rt_scene_cornell_box(void);

rt_hittable_list_t *rt_scene_instance_test(void);

rt_hittable_list_t *rt_scene_cornell_box_smoke_boxes(void);

rt_hittable_list_t *rt_scene_showcase(void);

rt_hittable_list_t *rt_scene_metal_test(void);

#endif // RAY_TRACING_ONE_WEEK_RT_SCENES_H
