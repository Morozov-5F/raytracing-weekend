/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef RAY_TRACING_ONE_WEEK_RT_MATRIX3_H
#define RAY_TRACING_ONE_WEEK_RT_MATRIX3_H

#include <rt_vec3.h>

typedef union rt_matrix3_u
{
    double matrix[3][3];
    struct
    {
        vec3_t columns[3];
    };
} rt_matrix3_t;

static inline vec3_t rt_mat3_mul_vec3(const rt_matrix3_t *a, const vec3_t *b)
{
    vec3_t new_vec = vec3(0., 0., 0.);
    for (int row = 0; row < 3; ++row)
    {
        new_vec.components[row] = a->matrix[0][row] * b->components[0] + a->matrix[1][row] * b->components[1] +
                                  a->matrix[2][row] * b->components[2];
    }
    return new_vec;
}

static inline rt_matrix3_t rt_mat3_mul(const rt_matrix3_t *a, const rt_matrix3_t *b)
{
    rt_matrix3_t res;
    for (int row = 0; row < 3; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            res.matrix[row][column] = a->matrix[0][row] * b->matrix[column][0] + a->matrix[1][row] * b->matrix[column][1] +
                                      a->matrix[2][row] * b->matrix[column][2];
        }
    }
    return res;
}

static inline rt_matrix3_t rt_matrix_identity(void)
{
    rt_matrix3_t result = {0};
    result.matrix[0][0] = result.matrix[1][1] = result.matrix[2][2] = 1;
    return result;
}

static inline rt_matrix3_t rt_matrix_rotation_x(double angle)
{
    rt_matrix3_t result = {.matrix = {{1, 0, 0}, {0, cos(angle), sin(angle)}, {0, -sin(angle), cos(angle)}}};
    return result;
}

static inline rt_matrix3_t rt_matrix_rotation_y(double angle)
{
    rt_matrix3_t result = {.matrix = {{cos(angle), 0, -sin(angle)}, {0, 1, 0}, {sin(angle), 0, cos(angle)}}};
    return result;
}

static inline rt_matrix3_t rt_matrix_rotation_z(double angle)
{
    rt_matrix3_t result = {.matrix = {{cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1}}};
    return result;
}

#endif // RAY_TRACING_ONE_WEEK_RT_MATRIX3_H
