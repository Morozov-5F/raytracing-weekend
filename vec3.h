#ifndef RAY_TRACING_ONE_WEEK_VEC3_H
#define RAY_TRACING_ONE_WEEK_VEC3_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define VEC3_STR_BUFFER_LENGTH (32)

struct vec3_s
{
    double e[3];
};

typedef struct vec3_s vec3_t;

static inline vec3_t vec3(double e1, double e2, double e3)
{
    vec3_t result = {.e = {e1, e2, e3}};
    return result;
}

static inline double vec3_x(const vec3_t *v)
{ return v->e[0]; }

static inline double vec3_y(const vec3_t *v)
{ return v->e[1]; }

static inline double vec3_z(const vec3_t *v)
{ return v->e[2]; }

static inline vec3_t vec3_negate(const vec3_t *v)
{ return vec3(-v->e[0], -v->e[1], -v->e[2]); }

static inline void vec3_add(vec3_t *a, const vec3_t *b)
{
    a->e[0] += b->e[0];
    a->e[1] += b->e[1];
    a->e[2] += b->e[2];
}

static inline void vec3_scale(vec3_t *a, double scalar)
{
    a->e[0] *= scalar;
    a->e[1] *= scalar;
    a->e[2] *= scalar;
}

static inline double vec3_length_squared(const vec3_t *v)
{ return v->e[0] * v->e[0] + v->e[1] + v->e[1] + v->e[2] * v->e[2]; }

static inline double vec3_length(const vec3_t *v)
{ return sqrt(vec3_length_squared(v)); }

static inline vec3_t vec3_sum(const vec3_t *a, const vec3_t *b)
{ return vec3(a->e[0] + b->e[0], a->e[1] + b->e[1], a->e[2] + b->e[2]); }

static inline vec3_t vec3_diff(const vec3_t *a, const vec3_t *b)
{ return vec3(a->e[0] - b->e[0], a->e[1] - b->e[1], a->e[2] - b->e[2]); }

static inline vec3_t vec3_multiply(const vec3_t *a, const vec3_t *b)
{ return vec3(a->e[0] * b->e[0], a->e[1] * b->e[1], a->e[2] * b->e[2]); }

static inline vec3_t vec3_multiply_scalar(const vec3_t *a, double s)
{ return vec3(a->e[0] * s, a->e[1] * s, a->e[2] * s); }

static inline double vec3_dor(const vec3_t *a, const vec3_t *b)
{ return a->e[0] * b->e[0] + a->e[1] * b->e[1] + a->e[2] * b->e[2]; }

static inline vec3_t vec3_cross(const vec3_t *a, const vec3_t *b)
{
    return vec3(a->e[1] * b->e[2] - a->e[2] * b->e[1],
                a->e[2] * b->e[0] - a->e[0] * b->e[2],
                a->e[0] * b->e[1] - a->e[1] * b->e[0]);
}

static inline vec3_t vec3_normalized(const vec3_t *a)
{
    return vec3_multiply_scalar(a, 1.0 / vec3_length(a));
}

static inline const char * vec3_to_string(const vec3_t *a, char *buffer, size_t max_buffer_len)
{
    if (-1 == snprintf(buffer, max_buffer_len, "%f %f %f", a->e[0], a->e[1], a->e[2]))
    {
        return NULL;
    }
    return buffer;
}

typedef vec3_t colour;
typedef vec3_t point;

#endif //RAY_TRACING_ONE_WEEK_VEC3_H
