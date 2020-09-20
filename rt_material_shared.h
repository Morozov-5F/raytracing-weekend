//
// Created by morozov on 16.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_SHARED_H

#include "rt_material.h"

typedef enum rt_material_type_e
{
    RT_MATERIAL_TYPE_UNKNOWN = 0,
    RT_MATERIAL_TYPE_DIFFUSE_LAMBERTIAN = 1,
    RT_MATERIAL_TYPE_METAL = 2,
    RT_MATERIAL_TYPE_DIELECTRIC = 3,
    RT_MATERIAL_TYPE_DIFFUSE_LIGHT = 4,
} rt_material_type_t;


struct rt_material_s
{
    rt_material_type_t type;
    int refcount;
};

void rt_material_base_init(rt_material_t *material_base, rt_material_type_t type);

#endif //RAY_TRACING_ONE_WEEK_RT_MATERIAL_SHARED_H
