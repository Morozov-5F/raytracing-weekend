/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_TEXTURE_SHARED_H
#define RAY_TRACING_ONE_WEEK_RT_TEXTURE_SHARED_H

#include <rt_texture.h>

void rt_texture_init(rt_texture_t *texture, rt_texture_type_t type, rt_texture_value_fn value_fn,
                     rt_texture_free_fn free_fn);

#endif // RAY_TRACING_ONE_WEEK_RT_TEXTURE_SHARED_H
