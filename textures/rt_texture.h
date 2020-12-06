/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_H

#include <rt_weekend.h>
#include <rt_colour.h>

typedef struct rt_texture_s rt_texture_t;

colour_t rt_texture_value(const rt_texture_t *texture, double u, double v, const vec3_t *p);

rt_texture_t *rt_texture_claim(rt_texture_t *texture);

void rt_texture_delete(rt_texture_t *texture);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_H
