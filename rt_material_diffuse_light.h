/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_LIGHT_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_LIGHT_H

#include <stdbool.h>
#include "rt_colour.h"
#include "rt_hit.h"

typedef struct rt_material_diffuse_light_s rt_material_diffuse_light_t;

rt_material_diffuse_light_t *rt_mt_diffuse_light_new(colour_t albedo);

bool
rt_mt_diffuse_light_scatter(const rt_material_diffuse_light_t *material, const ray_t *incoming_ray,
                            const rt_hit_record_t *hit_record,
                            colour_t *attenuation, ray_t *scattered_ray);

colour_t rt_mt_diffuse_light_emitted(const rt_material_diffuse_light_t *material);


void rt_mt_diffuse_light_delete(rt_material_diffuse_light_t *diffuse_light);

#endif //RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_LIGHT_H
