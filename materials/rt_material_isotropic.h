/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_ISOTROPIC_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_ISOTROPIC_H

#include <rt_colour.h>
#include <rt_texture.h>
#include <rt_weekend.h>
#include <rt_material.h>

typedef struct rt_material_iso_s rt_material_iso_t;

rt_material_iso_t *rt_mt_iso_new_with_albedo(colour_t albedo);
rt_material_iso_t *rt_mt_iso_new_with_texture(rt_texture_t *texture);

bool rt_mt_iso_scatter(const rt_material_iso_t *material, const ray_t *incoming_ray,
                      const rt_hit_record_t *hit_record, colour_t *attenuation, ray_t *scattered_ray);

void rt_mt_iso_delete(rt_material_iso_t *material);


#endif // RAY_TRACING_ONE_WEEK_RT_MATERIAL_ISOTROPIC_H
