/**
 * Copyright (c) 2021, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <assert.h>
#include "rt_weekend.h"
#include "rt_hittable.h"
#include "rt_hittable_shared.h"
#include "rt_hittable_list.h"

typedef struct rt_mesh_s
{
    rt_hittable_t base;

    rt_hittable_list_t *triangles;
    vec3_t min, max;
} rt_mesh_t;

static rt_mesh_t rt_mesh_init(const point3_t *vertices, size_t vertices_count, const size_t *triangles, size_t triangles_count, rt_material_t *material);
static bool rt_mesh_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                        rt_hit_record_t *record);
static bool rt_mesh_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb);
static void rt_mesh_delete(rt_hittable_t *hittable);

rt_hittable_t *rt_triangle_mesh_new(const point3_t *vertices, size_t vertices_count, const size_t *triangles, size_t triangles_count, rt_material_t *material)
{
    rt_mesh_t *mesh = (rt_mesh_t *)calloc(1, sizeof(rt_mesh_t));
    assert(NULL != mesh);

    *mesh = rt_mesh_init(vertices, vertices_count, triangles, triangles_count, material);
    return (rt_hittable_t *)mesh;
}

static rt_mesh_t rt_mesh_init(const point3_t *vertices, size_t vertices_count, const size_t *triangles, size_t triangles_count,
                              rt_material_t *material)
{
    assert(NULL != vertices);

    rt_mesh_t result = {
        .triangles = rt_hittable_list_init(triangles_count),
        .min = vec3(0, 0, 0),
        .max = vec3(0, 0, 0),
    };
    assert(NULL != triangles);

    for (size_t i = 0; i < triangles_count; ++i)
    {
        assert(triangles[3 * i] < vertices_count);
        point3_t a = vertices[triangles[3 * i]];

        assert(triangles[3 * i + 1] < vertices_count);
        point3_t b = vertices[triangles[3 * i + 1]];

        assert(triangles[3 * i + 2] < vertices_count);
        point3_t c = vertices[triangles[3 * i + 2]];

        result.min.x = fmin(result.min.x, fmin(a.x, fmin(b.x, c.x)));
        result.min.y = fmin(result.min.y, fmin(a.y, fmin(b.y, c.y)));
        result.min.z = fmin(result.min.z, fmin(a.z, fmin(b.z, c.z)));

        result.max.x = fmax(result.max.x, fmax(a.x, fmax(b.x, c.x)));
        result.max.y = fmax(result.max.y, fmax(a.y, fmax(b.y, c.y)));
        result.max.z = fmax(result.max.z, fmax(a.z, fmax(b.z, c.z)));

        rt_hittable_list_add(result.triangles, rt_triangle_new(a, b, c, rt_material_claim(material)));
    }

    rt_material_delete(material);

    rt_hittable_init(&result.base, RT_HITTABLE_TYPE_MESH, rt_mesh_hit, rt_mesh_bb, rt_mesh_delete);
    return result;
}

static bool rt_mesh_hit(const rt_hittable_t *hittable, const ray_t *ray, double t_min, double t_max,
                        rt_hit_record_t *record)
{
    assert(NULL != hittable);
    assert(RT_HITTABLE_TYPE_MESH == hittable->type);

    const rt_mesh_t *mesh = (const rt_mesh_t *)hittable;
    return rt_hittable_list_hit_test(mesh->triangles, ray, t_min, t_max, record);
}

static bool rt_mesh_bb(const rt_hittable_t *hittable, double time0, double time1, rt_aabb_t *out_bb)
{
    assert(NULL != hittable);
    assert(RT_HITTABLE_TYPE_MESH == hittable->type);

    const rt_mesh_t *mesh = (const rt_mesh_t *)hittable;
    *out_bb = rt_aabb(mesh->min, mesh->max);

    return true;
}

static void rt_mesh_delete(rt_hittable_t *hittable)
{
    if (NULL == hittable)
    {
        return;
    }
    assert(RT_HITTABLE_TYPE_MESH == hittable->type);

    const rt_mesh_t *mesh = (const rt_mesh_t *)hittable;
    rt_hittable_list_deinit(mesh->triangles);
}

