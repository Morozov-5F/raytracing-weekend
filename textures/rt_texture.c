/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "rt_texture.h"
#include <rt_texture_shared.h>
#include <assert.h>

static colour_t rt_texture_value_default(const rt_texture_t *texture, double u, double v, const vec3_t *p);
static void rt_texture_delete_default(rt_texture_t *texture);

rt_texture_t *rt_texture_claim(rt_texture_t *texture)
{
    assert(NULL != texture);

    texture->refcount++;
    return texture;
}

void rt_texture_init(rt_texture_t *texture, rt_texture_type_t type, rt_texture_value_fn value_fn,
                     rt_texture_free_fn free_fn)
{
    assert(NULL != texture);
    texture->refcount = 1;
    texture->type = type;

    if (NULL == value_fn)
    {
        value_fn = rt_texture_value_default;
    }
    texture->get_value = value_fn;

    if (NULL == free_fn)
    {
        free_fn = rt_texture_delete_default;
    }
    texture->free = free_fn;
}

colour_t rt_texture_value(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    assert(NULL != texture);

    return texture->get_value(texture, u, v, p);
}

void rt_texture_delete(rt_texture_t *texture)
{
    if (NULL != texture && --texture->refcount > 0)
    {
        return;
    }
    texture->free(texture);
}

static colour_t rt_texture_value_default(const rt_texture_t *texture, double u, double v, const vec3_t *p)
{
    assert(0);
}

static void rt_texture_delete_default(rt_texture_t *texture)
{
    assert(0);
}
