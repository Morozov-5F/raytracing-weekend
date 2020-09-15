//
// Created by morozov on 15.09.2020.
//

#include <assert.h>
#include "rt_hittable_list.h"

void rt_hittable_list_init(rt_hittable_list_t *list, size_t capacity)
{
    assert(NULL != list);
    list->hittables = calloc(capacity, sizeof(rt_hittable_t *));
    assert(NULL != list->hittables);

    list->capacity = capacity;
    list->size = 0;
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
}

bool rt_hittable_list_hit_test(const rt_hittable_list_t *list, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record)
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
