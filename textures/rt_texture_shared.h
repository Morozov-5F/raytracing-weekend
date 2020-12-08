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

struct rt_texture_s
{
    rt_texture_type_t type;
    int refcount;
};

void rt_texture_init(rt_texture_t *texture, rt_texture_type_t type);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_SHARED_H
