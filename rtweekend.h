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

#define DEG_TO_RAD(deg) (((deg) * PI) / 180.0)

static inline double random_double(double min, double max)
{
    return min + (max - min) * rand() / (RAND_MAX + 1.0);
}

#include "vec3.h"
#include "ray.h"

#endif //RAY_TRACING_ONE_WEEK_RTWEEKEND_H
