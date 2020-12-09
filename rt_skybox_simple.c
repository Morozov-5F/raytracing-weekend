/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_skybox_simple.h"

struct rt_skybox_s
{
    colour_t from;
    colour_t to;
};

rt_skybox_t *rt_skybox_new_gradient(colour_t from, colour_t to)
{
    rt_skybox_t *result = calloc(1, sizeof(rt_skybox_t));
    assert(NULL != result);

    result->from = from;
    result->to = to;

    return result;
}

rt_skybox_t *rt_skybox_new_background(colour_t background)
{
    return rt_skybox_new_gradient(background, background);
}

colour_t rt_skybox_value(rt_skybox_t *skybox, const ray_t *ray)
{
    assert(NULL != skybox);
    assert(NULL != ray);

    point3_t unit_direction = vec3_normalized(ray->direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return vec3_lerp(skybox->from, skybox->to, t);
}

void rt_skybox_delete(rt_skybox_t *skybox)
{
    free(skybox);
}
