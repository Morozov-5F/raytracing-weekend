//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_HITTABLE_LIST_H
#define RAY_TRACING_ONE_WEEK_RT_HITTABLE_LIST_H

#include <stdlib.h>
#include "rtweekend.h"
#include "rt_hittable.h"

typedef struct rt_hittable_list_s
{
    rt_hittable_t **hittables;
    size_t size;
    size_t capacity;
} rt_hittable_list_t;

void rt_hittable_list_init(rt_hittable_list_t *list, size_t capacity);

void rt_hittable_list_add(rt_hittable_list_t *list, rt_hittable_t *hittable);
void rt_hittable_list_clear(rt_hittable_list_t *list);
void rt_hittable_list_deinit(rt_hittable_list_t *list);

bool rt_hittable_list_hit_test(const rt_hittable_list_t *list, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record);

#endif //RAY_TRACING_ONE_WEEK_RT_HITTABLE_LIST_H
