/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_SKYBOX_SIMPLE_H
#define RAY_TRACING_ONE_WEEK_RT_SKYBOX_SIMPLE_H

#include "rt_colour.h"
typedef struct rt_skybox_s rt_skybox_t;

rt_skybox_t *rt_skybox_new_gradient(colour_t from, colour_t to);

rt_skybox_t *rt_skybox_new_background(colour_t background);

colour_t rt_skybox_value(rt_skybox_t *skybox, const ray_t *ray);

void rt_skybox_delete(rt_skybox_t *skybox);

#endif // RAY_TRACING_ONE_WEEK_RT_SKYBOX_SIMPLE_H
