//
// Created by morozov on 16.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_H

#include <stdbool.h>
#include "rt_hit.h"
#include "rt_colour.h"

typedef struct rt_material_s rt_material_t;

void rt_material_claim(rt_material_t *material);

bool rt_material_scatter(const rt_material_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                         colour_t *attenuation, ray_t *scattered_ray);

colour_t rt_material_emitted(const rt_material_t *material);

void rt_material_delete(rt_material_t *material);

#endif //RAY_TRACING_ONE_WEEK_RT_MATERIAL_H
