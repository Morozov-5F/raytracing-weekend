/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_IMAGE_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_IMAGE_H

#include <rt_weekend.h>
#include <rt_texture.h>

typedef struct rt_texture_image_s rt_texture_image_t;

rt_texture_image_t *rt_texture_image_new(const char *filename);

colour_t rt_texture_image_value(const rt_texture_image_t *texture_image, double u, double v, const vec3_t *p);

void rt_texture_image_delete(rt_texture_image_t *texture_image);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_IMAGE_H
