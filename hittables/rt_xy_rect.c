/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_xy_rect.h"
#include <rt_hittable_shared.h>
#include <assert.h>

struct rt_xy_rect_s
{
    rt_hittable_t base;
    rt_material_t *material;
    double x0, y0, x1, y1, k;
};

rt_xy_rect_t *rt_xy_rect_new(point3_t top_left, double width, double height, rt_material_t *material)
{
    rt_xy_rect_t *result = calloc(1, sizeof(rt_xy_rect_t));
    assert(NULL != result);

    result->material = material;
    result->x0 = top_left.x;
    result->y0 = top_left.y;
    result->k = top_left.z;
    result->x1 = result->x0 + width;
    result->y1 = result->y0 + height;

    rt_hittable_init(&result->base, RT_HITTABLE_TYPE_XY_RECT);

    return result;
}

void rt_xy_rect_delete(rt_xy_rect_t *xy_rect)
{
    if (NULL == xy_rect)
    {
        return;
    }
    rt_material_delete(xy_rect->material);
    free(xy_rect);
}

bool rt_xy_rect_hit(const rt_xy_rect_t *xy_rect, const ray_t *ray, double t_min, double t_max, rt_hit_record_t *record)
{
    assert(NULL != xy_rect);
    assert(NULL != ray);

    double t = (xy_rect->k - ray->origin.z) / ray->direction.z;
    if (t >= t_max || t <= t_min)
    {
        return false;
    }

    point3_t hit = ray_at(*ray, t);
    if (hit.x < xy_rect->x0 || hit.x > xy_rect->x1 || hit.y < xy_rect->y0 || hit.y > xy_rect->y1)
    {
        return false;
    }

    if (NULL != record)
    {
        record->material = xy_rect->material;
        record->u = (hit.x - xy_rect->x0) / (xy_rect->x1 - xy_rect->x0);
        record->v = (hit.y - xy_rect->y0) / (xy_rect->y1 - xy_rect->y0);
        record->t = t;
        record->p = hit;

        vec3_t outward_normal = vec3(0, 0, 1);
        rt_hit_record_set_front_face(record, ray, &outward_normal);
    }

    return true;
}

bool rt_xy_rect_bb(const rt_xy_rect_t *xy_rect, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != xy_rect);
    assert(NULL != out_bb);

    *out_bb = rt_aabb(point3(xy_rect->x0, xy_rect->y0, xy_rect->k - 0.0001),
                      point3(xy_rect->x1, xy_rect->y1, xy_rect->k + 0.0001));

    return true;
}
