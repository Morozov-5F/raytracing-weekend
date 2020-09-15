//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RTWEEKEND_H
#define RAY_TRACING_ONE_WEEK_RTWEEKEND_H

#include <stdlib.h>
#include <limits.h>
#include <math.h>

#ifdef M_PI
#define PI M_PI
#else
#define PI (double)3.1415926535897932385
#endif // M_PI

#define RT_DEG_TO_RAD(deg) (((deg) * PI) / 180.0)

static inline double rt_random_double(double min, double max)
{
    return min + (max - min) * rand() / (RAND_MAX + 1.0);
}

#include "rt_vec3.h"
#include "rt_ray.h"

#endif //RAY_TRACING_ONE_WEEK_RTWEEKEND_H
