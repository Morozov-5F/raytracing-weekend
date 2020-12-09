/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_LIGHT_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_LIGHT_H

#include <rt_colour.h>
#include <rt_texture.h>
#include <rt_weekend.h>
#include <rt_material.h>

typedef struct rt_material_dl_s rt_material_dl_t;

rt_material_dl_t *rt_mt_dl_new_with_albedo(colour_t albedo, double intensity);
rt_material_dl_t *rt_mt_dl_new_with_texture(rt_texture_t *texture, double intensity);

bool rt_mt_dl_scatter(const rt_material_dl_t *material, const ray_t *incoming_ray,
                           const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray);

colour_t rt_mt_dl_emit(const rt_material_dl_t *material, double u, double v, const point3_t *p);

void rt_mt_dl_delete(rt_material_dl_t *material);

#endif // RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_LIGHT_H
