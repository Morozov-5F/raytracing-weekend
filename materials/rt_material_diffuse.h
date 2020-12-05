/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_H

#include "../rt_colour.h"
#include "rt_material.h"
#include <stdbool.h>

typedef struct rt_material_diffuse_s rt_material_diffuse_t;

rt_material_diffuse_t *rt_mt_diffuse_new(colour_t albedo);

bool
rt_mt_diffuse_scatter(const rt_material_diffuse_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                      colour_t *attenuation, ray_t *scattered_ray);

void rt_mt_diffuse_delete(rt_material_diffuse_t *diffuse);

#endif //RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIFFUSE_H
