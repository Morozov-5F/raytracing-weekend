/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_MATERIAL_H
#define RAY_TRACING_ONE_WEEK_RT_MATERIAL_H

#include <stdbool.h>

#include <rt_hit.h>
#include <rt_colour.h>
#include <rt_texture.h>

typedef struct rt_material_s rt_material_t;

rt_material_t *rt_material_claim(rt_material_t *material);

bool rt_material_scatter(const rt_material_t *material, const ray_t *incoming_ray, const rt_hit_record_t *hit_record,
                         colour_t *attenuation, ray_t *scattered_ray);

colour_t rt_material_emit(const rt_material_t *material, double u, double v, const point3_t *p);

void rt_material_delete(rt_material_t *material);

// Dielectric material
rt_material_t *rt_mt_dielectric_new(double refraction_factor);

// Diffuse material
rt_material_t *rt_mt_diffuse_new_with_albedo(colour_t albedo);
rt_material_t *rt_mt_diffuse_new_with_texture(rt_texture_t *texture);

// Diffuse light material
rt_material_t *rt_mt_dl_new_with_albedo(colour_t albedo, double intensity);
rt_material_t *rt_mt_dl_new_with_texture(rt_texture_t *texture, double intensity);

// Isotropic material
rt_material_t *rt_mt_iso_new_with_albedo(colour_t albedo);
rt_material_t *rt_mt_iso_new_with_texture(rt_texture_t *texture);

// Metal material
rt_material_t *rt_mt_metal_new(colour_t albedo, double fuzziness);

#endif // RAY_TRACING_ONE_WEEK_RT_MATERIAL_H
