/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_VEC3_H
#define RAY_TRACING_ONE_WEEK_RT_VEC3_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define VEC3_STR_BUFFER_LENGTH (32)

typedef enum vec3_axis_e
{
    VEC3_AXIS_X = 0,
    VEC3_AXIS_Y = 1,
    VEC3_AXIS_Z = 2,
} vec3_axis_t;

union vec3_u
{
    double components[3];
    struct {
        double x;
        double y;
        double z;
    };
};

typedef union vec3_u vec3_t;

static inline vec3_t vec3(double x, double y, double z)
{
    vec3_t result = {.x = x, .y = y, .z = z};
    return result;
}

static inline vec3_t vec3_negate(const vec3_t *v)
{
    return vec3(-v->x, -v->y, -v->z);
}

static inline void vec3_add(vec3_t *a, vec3_t b)
{
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}

static inline void vec3_sub(vec3_t *a, vec3_t b)
{
    a->x -= b.x;
    a->y -= b.y;
    a->z -= b.z;
}

static inline void vec3_scale_in_place(vec3_t *a, double scalar)
{
    a->x *= scalar;
    a->y *= scalar;
    a->z *= scalar;
}

static inline double vec3_length_squared(vec3_t v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline double vec3_length(vec3_t v)
{
    return sqrt(vec3_length_squared(v));
}

static inline vec3_t vec3_sum(vec3_t a, vec3_t b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline vec3_t vec3_diff(vec3_t a, vec3_t b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline vec3_t vec3_multiply(vec3_t a, vec3_t b)
{
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

static inline vec3_t vec3_scale(vec3_t a, double s)
{
    return vec3(a.x * s, a.y * s, a.z * s);
}

static inline double vec3_dot(vec3_t a, vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline vec3_t vec3_cross(vec3_t a, vec3_t b)
{
    return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline vec3_t vec3_normalized(vec3_t a)
{
    return vec3_scale(a, 1.0 / vec3_length(a));
}

static inline const char *vec3_to_string(vec3_t a, char *buffer, size_t max_buffer_len)
{
    if (-1 == snprintf(buffer, max_buffer_len, "%f %f %f", a.x, a.y, a.z))
    {
        return NULL;
    }
    return buffer;
}

static inline vec3_t vec3_lerp(vec3_t from, vec3_t to, double t)
{
    return vec3_sum(vec3_scale(from, 1 - t), vec3_scale(to, t));
}

static inline vec3_t vec3_random(double min, double max)
{
    return vec3(rt_random_double(min, max), rt_random_double(min, max), rt_random_double(min, max));
}

static inline vec3_t vec3_random_unit_vector(void)
{
    double a = rt_random_double(0, 2 * PI);
    double z = rt_random_double(-1, 1);
    double r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}

static inline vec3_t vec3_random_in_unit_sphere(void)
{
    vec3_t res = vec3_random(-1, 1);
    if (vec3_length_squared(res) > 1)
    {
        res = vec3_normalized(res);
    }
    return res;
}

static inline vec3_t vec3_random_in_hemisphere(const vec3_t *n)
{
    vec3_t in_unit_sphere = vec3_random_in_unit_sphere();
    if (vec3_dot(in_unit_sphere, *n) > 0)
    {
        return in_unit_sphere;
    }
    return vec3_negate(&in_unit_sphere);
}

static inline vec3_t vec3_random_in_unit_disc(void)
{
    vec3_t res = vec3(rt_random_double(-1, 1), rt_random_double(-1, 1), 0);
    if (vec3_length_squared(res) > 1)
    {
        return vec3_normalized(res);
    }
    return res;
}

static inline vec3_t vec3_reflect(const vec3_t *vec, const vec3_t *n)
{
    double scale = 2 * vec3_dot(*vec, *n);
    return vec3_diff(*vec, vec3_scale(*n, scale));
}

static inline vec3_t vec3_refract(const vec3_t *vec, const vec3_t *n, double r)
{
    double vec_length = vec3_length(*vec);
    double c = -vec3_dot(*vec, *n) / vec_length;

    double aux = vec_length * (r * c - sqrt(1 - r * r * (1 - c * c)));
    return vec3_sum(vec3_scale(*vec, r), vec3_scale(*n, aux));
}

// Point layer for vec3_t
typedef vec3_t point3_t;
#define point3(x, y, z) vec3((x), (y), (z))

#endif // RAY_TRACING_ONE_WEEK_RT_VEC3_H
