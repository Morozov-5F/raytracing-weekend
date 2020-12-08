/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_PERLIN_H
#define RAY_TRACING_ONE_WEEK_RT_PERLIN_H

#include "rt_weekend.h"

typedef struct rt_perlin_s rt_perlin_t;

rt_perlin_t *rt_perlin_new(void);

double rt_perlin_noise(const rt_perlin_t *perlin, point3_t point);

double rt_perlin_blocky_noise(const rt_perlin_t *perlin, point3_t point);

double rt_perlin_turbulence(const rt_perlin_t *perlin, point3_t p, int depth);

void rt_perlin_delete(rt_perlin_t *perlin);

#endif // RAY_TRACING_ONE_WEEK_RT_PERLIN_H
