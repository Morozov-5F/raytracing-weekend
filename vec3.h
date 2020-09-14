#ifndef RAY_TRACING_ONE_WEEK_VEC3_H
#define RAY_TRACING_ONE_WEEK_VEC3_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define VEC3_STR_BUFFER_LENGTH (32)

struct vec3_s
{
    double x;
    double y;
    double z;
};

typedef struct vec3_s vec3_t;

static inline vec3_t vec3(double x, double y, double z)
{
    vec3_t result = {.x = x, .y = y, .z = z};
    return result;
}

static inline vec3_t vec3_negate(const vec3_t *v)
{ return vec3(-v->x, -v->y, -v->z); }

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
{ return v.x * v.x + v.y * v.y + v.z * v.z; }

static inline double vec3_length(vec3_t v)
{ return sqrt(vec3_length_squared(v)); }

static inline vec3_t vec3_sum(vec3_t a, vec3_t b)
{ return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }

static inline vec3_t vec3_diff(vec3_t a, vec3_t b)
{ return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }

static inline vec3_t vec3_multiply(vec3_t a, vec3_t b)
{ return vec3(a.x * b.x, a.y * b.y, a.z * b.z); }

static inline vec3_t vec3_scale(vec3_t a, double s)
{ return vec3(a.x * s, a.y * s, a.z * s); }

static inline double vec3_dor(vec3_t a, vec3_t b)
{ return a.x * b.x + a.y * b.y + a.z * b.z; }

static inline vec3_t vec3_cross(vec3_t a, vec3_t b)
{
    return vec3(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

static inline vec3_t vec3_normalized(vec3_t a)
{
    return vec3_scale(a, 1.0 / vec3_length(a));
}

static inline const char * vec3_to_string(vec3_t a, char *buffer, size_t max_buffer_len)
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

typedef vec3_t colour_t;
typedef vec3_t point3_t;

#endif //RAY_TRACING_ONE_WEEK_VEC3_H
