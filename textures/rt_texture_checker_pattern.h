/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_CHECKER_PATTERN_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_CHECKER_PATTERN_H
#include <rt_texture.h>

typedef struct rt_texture_cp_s rt_texture_cp_t;

rt_texture_cp_t *rt_texture_cp_new_with_textures(rt_texture_t *even, rt_texture_t *odd);

rt_texture_cp_t *rt_texture_cp_new_with_colour(colour_t colour_even, colour_t colour_odd);

colour_t rt_texture_cp_value(const rt_texture_cp_t *texture_cp, double u, double v, const vec3_t *p);

void rt_texture_cp_delete(rt_texture_cp_t *texture_cp);
#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_CHECKER_PATTERN_H
