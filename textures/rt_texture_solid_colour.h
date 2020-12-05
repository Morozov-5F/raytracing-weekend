/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_SOLID_COLOUR_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_SOLID_COLOUR_H

#include <rt_texture.h>

typedef struct rt_texture_sc_s rt_texture_sc_t;

rt_texture_sc_t *rt_texture_sc_new(double red, double green, double blue);

colour_t rt_texture_sc_value(const rt_texture_sc_t *texture_sc, double u, double v, const vec3_t *p);

void rt_texture_sc_delete(rt_texture_sc_t *texture_sc);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_SOLID_COLOUR_H
