/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_SHARED_H

#include <rt_texture.h>

typedef enum rt_texture_type_e
{
    RT_TEXTURE_TYPE_NONE = -1,
    RT_TEXTURE_TYPE_SOLID_COLOUR,
    RT_TEXTURE_TYPE_CHECKER,
    RT_TEXTURE_TYPE_NOISE,
    RT_TEXTURE_TYPE_IMAGE,
} rt_texture_type_t;

typedef colour_t (*rt_texture_value_fn)(const rt_texture_t *texture, double u, double v, const vec3_t *p);
typedef void (*rt_texture_free_fn)(rt_texture_t *texture);

struct rt_texture_s
{
    rt_texture_type_t type;
    int refcount;

    rt_texture_value_fn get_value;
    rt_texture_free_fn free;
};

void rt_texture_init(rt_texture_t *texture, rt_texture_type_t type, rt_texture_value_fn value_fn,
                     rt_texture_free_fn free_fn);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_SHARED_H
