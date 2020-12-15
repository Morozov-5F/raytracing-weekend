/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_H

#include <rt_weekend.h>
#include <rt_colour.h>

typedef enum rt_texture_type_e
{
    RT_TEXTURE_TYPE_NONE = -1,
    RT_TEXTURE_TYPE_SOLID_COLOUR,
    RT_TEXTURE_TYPE_CHECKER,
    RT_TEXTURE_TYPE_NOISE,
    RT_TEXTURE_TYPE_IMAGE,
} rt_texture_type_t;

typedef struct rt_texture_s rt_texture_t;

typedef colour_t (*rt_texture_value_fn)(const rt_texture_t *texture, double u, double v, const vec3_t *p);
typedef void (*rt_texture_free_fn)(rt_texture_t *texture);

struct rt_texture_s
{
    rt_texture_type_t type;
    int refcount;

    rt_texture_value_fn get_value;
    rt_texture_free_fn free;
};

rt_texture_t *rt_texture_claim(rt_texture_t *texture);

// Solid colour constructors
rt_texture_t *rt_texture_sc_new(colour_t colour);
rt_texture_t *rt_texture_sc_new_with_components(double red, double green, double blue);

// Checker pattern constructors
rt_texture_t *rt_texture_cp_new_with_colour(colour_t colour_even, colour_t colour_odd);
rt_texture_t *rt_texture_cp_new_with_textures(rt_texture_t *even, rt_texture_t *odd);

// Image texture constructors
rt_texture_t *rt_texture_image_new(const char *filename);

// Noise texture constructors
rt_texture_t *rt_texture_noise_new(double intensity);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_H
