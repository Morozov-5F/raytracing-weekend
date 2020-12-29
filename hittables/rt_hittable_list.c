/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include "rt_hittable_list.h"

struct rt_hittable_list_s
{
    rt_hittable_t **hittables;
    size_t size;
    size_t capacity;
};

rt_hittable_list_t *rt_hittable_list_init(size_t capacity)
{
    rt_hittable_list_t *list = calloc(1, sizeof(rt_hittable_list_t));
    assert(NULL != list);

    list->hittables = calloc(capacity, sizeof(rt_hittable_t *));
    assert(NULL != list->hittables);

    list->capacity = capacity;
    list->size = 0;

    return list;
}

void rt_hittable_list_add(rt_hittable_list_t *list, rt_hittable_t *hittable)
{
    assert(NULL != list);

    if (list->size > list->capacity)
    {
        list->hittables = realloc(list->hittables, list->capacity * 2 * sizeof(rt_hittable_t *));
        assert(NULL != list->hittables);
        list->capacity = list->capacity * 2;
    }
    list->hittables[list->size++] = hittable;
}

void rt_hittable_list_clear(rt_hittable_list_t *list)
{
    assert(NULL != list);

    for (size_t i = 0; i < list->size; ++i)
    {
        rt_hittable_delete(list->hittables[i]);
    }
    list->size = 0;
}

void rt_hittable_list_deinit(rt_hittable_list_t *list)
{
    assert(NULL != list);

    rt_hittable_list_clear(list);

    free(list->hittables);
    list->capacity = 0;
    list->hittables = NULL;

    free(list);
}

bool rt_hittable_list_hit_test(const rt_hittable_list_t *list, const ray_t *ray, double t_min, double t_max,
                               rt_hit_record_t *record)
{
    assert(NULL != list);
    assert(NULL != ray);

    if (NULL == list->hittables)
    {
        return false;
    }

    assert(list->size <= list->capacity);

    bool hit_occurred = false;
    double closest_t_so_far = t_max;
    rt_hit_record_t temp_record;

    for (size_t i = 0; i < list->size; ++i)
    {
        if (rt_hittable_hit(list->hittables[i], ray, t_min, closest_t_so_far, &temp_record))
        {
            hit_occurred = true;
            closest_t_so_far = temp_record.t;
            *record = temp_record;
        }
    }

    return hit_occurred;
}

bool rt_hittable_list_bb(const rt_hittable_list_t *list, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != list);
    assert(NULL != out_bb);

    if (list->size == 0)
    {
        return false;
    }

    rt_aabb_t temp_box;
    bool first_box = true;
    for (size_t i = 0; i < list->size; ++i)
    {
        if (!rt_hittable_bb(list->hittables[i], time0, time1, &temp_box))
        {
            return false;
        }

        if (first_box)
        {
            *out_bb = temp_box;
            first_box = false;
            continue;
        }

        *out_bb = rt_aabb_surrounding_bb(*out_bb, temp_box);
    }

    return true;
}

size_t rt_hittable_list_get_size(const rt_hittable_list_t *list)
{
    assert(NULL != list);

    return list->size;
}

rt_hittable_t **rt_hittable_list_get_underlying_container(const rt_hittable_list_t *list)
{
    assert(NULL != list);

    return list->hittables;
}
