/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_NOISE_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_NOISE_H

#include <rt_texture.h>

typedef struct rt_texture_noise_s rt_texture_noise_t;

rt_texture_noise_t *rt_texture_noise_new(void);

colour_t rt_texture_noise_value(const rt_texture_noise_t *texture_noise, double u, double v, const vec3_t *p);

void rt_texture_noise_delete(rt_texture_noise_t *texture_noise);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_NOISE_H
