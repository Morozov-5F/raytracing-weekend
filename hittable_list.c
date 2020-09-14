//
// Created by morozov on 15.09.2020.
//

#include <assert.h>
#include "hittable_list.h"

void hittable_list_init(hittable_list_t *list, size_t capacity)
{
    assert(NULL != list);
    list->hittables = calloc(capacity, sizeof(hittable_t *));
    assert(NULL != list->hittables);

    list->capacity = capacity;
    list->size = 0;
}

void hittable_list_add(hittable_list_t *list, hittable_t *hittable)
{
    assert(NULL != list);

    if (list->size > list->capacity)
    {
        list->hittables = realloc(list->hittables, list->capacity * 2 * sizeof(hittable_t *));
        assert(NULL != list->hittables);
        list->capacity = list->capacity * 2;
    }
    list->hittables[list->size++] = hittable;
}

void hittable_list_clear(hittable_list_t *list)
{
    assert(NULL != list);

    for (size_t i = 0; i < list->size; ++i)
    {
        hittable_delete(list->hittables[i]);
    }
    list->size = 0;
}

void hittable_list_deinit(hittable_list_t *list)
{
    assert(NULL != list);

    hittable_list_clear(list);

    free(list->hittables);
    list->capacity = 0;
    list->hittables = NULL;
}

bool hittable_list_hit_test(const hittable_list_t *list, const ray_t *ray, double t_min, double t_max, hit_record_t *record)
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
    hit_record_t temp_record;

    for (size_t i = 0; i < list->size; ++i)
    {
        if (hittable_hit(list->hittables[i], ray, t_min, closest_t_so_far, &temp_record))
        {
            hit_occurred = true;
            closest_t_so_far = temp_record.t;
            *record = temp_record;
        }
    }

    return hit_occurred;
}
