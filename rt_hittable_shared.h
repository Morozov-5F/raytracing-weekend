//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H

#include <stdbool.h>

typedef enum rt_hittable_type_e
{
    RT_HITTABLE_TYPE_UNKNOWN = -1,
    RT_HITTABLE_TYPE_SPHERE = 0,
} rt_hittable_type_t;

struct rt_hittable_s
{
    rt_hittable_type_t type;
};

#endif //RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
