/**
 * Copyright (c) 2021, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <stdlib.h>
#include <assert.h>
#include "rt_random.h"

struct rt_mt19937_s
{
    int w, n, m, r;

    uint64_t a;
    uint64_t u, d;
    uint64_t s, b;
    uint64_t t, c;

    int l;
    uint64_t f;

    uint64_t upper_mask, lower_mask;

    uint64_t index;
    uint64_t *MT;
};

static struct rt_mt19937_s gs_generator;
static void twist(struct rt_mt19937_s *gen);

void rt_random_seed(uint64_t seed)
{
    gs_generator.w = 64;
    gs_generator.n = 312;
    gs_generator.m = 156;
    gs_generator.r = 31;

    gs_generator.a = 0xB5026F5AA96619E9ULL;

    gs_generator.u = 29;
    gs_generator.d = 0x5555555555555555ULL;

    gs_generator.s = 17;
    gs_generator.b = 0x71D67FFFEDA60000;

    gs_generator.t = 37;
    gs_generator.c = 0xFFF7EEE000000000;

    gs_generator.l = 43;

    gs_generator.f = 6364136223846793005;

    gs_generator.lower_mask = (1 << gs_generator.r) - 1;
    gs_generator.upper_mask = ~gs_generator.lower_mask;

    gs_generator.MT = malloc(sizeof(*gs_generator.MT) * gs_generator.n);
    assert(NULL != gs_generator.MT);

    gs_generator.index = gs_generator.n;
    gs_generator.MT[0] = seed;

    for (int i = 1; i < gs_generator.n; ++i)
    {
        gs_generator.MT[i] = gs_generator.f * (gs_generator.MT[i - 1] ^ (gs_generator.MT[i - 1] >> (gs_generator.w - 2))) + i;
    }
}

uint64_t rt_random(void)
{
    if (gs_generator.index >= gs_generator.n)
    {
        twist(&gs_generator);
    }

    uint64_t y = gs_generator.MT[gs_generator.index++];
    y ^= (y >> gs_generator.u) & gs_generator.d;
    y ^= (y << gs_generator.s) & gs_generator.b;
    y ^= (y << gs_generator.t) & gs_generator.c;
    y ^= y >> gs_generator.l;

    return y;
}

static void twist(struct rt_mt19937_s *gen)
{
    assert(NULL != gen);

    for (int i = 0; i < gen->n; ++i)
    {
        uint64_t x = (gen->MT[i] & gen->upper_mask) + (gen->MT[(i + 1) % gen->n] & gen->lower_mask);
        uint64_t xA = x >> 1;

        if (x % 2 != 0)
        {
            xA ^= gen->a;
        }
        gen->MT[i] = gen->MT[(i + gen->m) % gen->n] ^ xA;
    }
    gen->index = 0;
}
