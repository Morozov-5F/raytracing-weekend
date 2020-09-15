//
// Created by morozov on 15.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_HITTABLE_LIST_H
#define RAY_TRACING_ONE_WEEK_HITTABLE_LIST_H

#include <stdlib.h>
#include "rtweekend.h"
#include "hittable.h"

typedef struct hittable_list_s
{
    hittable_t **hittables;
    size_t size;
    size_t capacity;
} hittable_list_t;

void hittable_list_init(hittable_list_t *list, size_t capacity);

void hittable_list_add(hittable_list_t *list, hittable_t *hittable);
void hittable_list_clear(hittable_list_t *list);
void hittable_list_deinit(hittable_list_t *list);

bool hittable_list_hit_test(const hittable_list_t *list, const ray_t *ray, double t_min, double t_max, hit_record_t *record);

#endif //RAY_TRACING_ONE_WEEK_HITTABLE_LIST_H
