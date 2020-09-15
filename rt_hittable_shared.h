//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H

#include <stdbool.h>

typedef enum hittable_type_e
{
    HITTABLE_TYPE_UNKNOWN = -1,
    HITTABLE_TYPE_SPHERE = 0,
} hittable_type_t;

struct hittable_s
{
    hittable_type_t type;
};

#endif //RAY_TRACING_ONE_WEEK_RT_HITTABLE_SHARED_H
