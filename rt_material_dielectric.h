//
// Created by morozov on 17.09.2020.
//

#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIELECTRIC_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIELECTRIC_H

#include <stdbool.h>
#include "rt_colour.h"
#include "rt_material.h"

typedef struct rt_material_dielectric_s rt_material_dielectric_t;

rt_material_dielectric_t *rt_mt_dielectric_new(double refraction_factor);

bool rt_mt_dielectric_scatter(const rt_material_dielectric_t *material, const ray_t *incoming_ray,
                              const rt_hit_record_t *hit_record,
                              colour_t *attenuation, ray_t *scattered_ray);

void rt_mt_dielectric_delete(rt_material_dielectric_t *dielectric);

#endif //RAY_TRACING_ONE_WEEK_RT_MATERIAL_DIELECTRIC_H