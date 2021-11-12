/**
 * Copyright (c) 2021, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_RANDOM_H
#define RAY_TRACING_ONE_WEEK_RT_RANDOM_H

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define RT_RANDOM_MAX UINT64_MAX

void rt_random_seed(uint64_t seed);

uint64_t rt_random(void);

#endif // RAY_TRACING_ONE_WEEK_RT_RANDOM_H
