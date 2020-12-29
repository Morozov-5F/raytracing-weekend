/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_WEEKEND_H
#define RAY_TRACING_ONE_WEEK_RT_WEEKEND_H

#include <stdlib.h>
#include <limits.h>
#include <math.h>

#ifdef M_PI
#define PI M_PI
#else
#define PI (double)3.1415926535897932385
#endif // M_PI

#define RT_DEG_TO_RAD(deg) (((deg)*PI) / 180.0)

static inline double rt_random_double(double min, double max)
{
    return min + (max - min) * rand() / (RAND_MAX + 1.0);
}

static inline double rt_clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

#include "rt_vec3.h"
#include "rt_ray.h"
#include "rt_matrix3.h"

#endif // RAY_TRACING_ONE_WEEK_RT_WEEKEND_H
