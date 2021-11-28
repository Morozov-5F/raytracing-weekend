/**
 * Copyright (c) 2020, Evgeniy Morozov
 * All rights reserved.
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RAY_TRACING_ONE_WEEK_RT_QUEUE_H
#define RAY_TRACING_ONE_WEEK_RT_QUEUE_H

// Minimal implementation of singly-linked generic queue. The implementation is almost identical to the one from OpenBSD
// project (see sys/queue.h in OpenBSD, STAILQ_*).

#define RT_QUEUE_HEAD(name, type)                                                                                      \
    struct name                                                                                                        \
    {                                                                                                                  \
        struct type *first; /* first element */                                                                        \
        struct type **last; /* address of last next element */                                                         \
    }

#define RT_QUEUE_ENTRY(type)                                                                                           \
    struct                                                                                                             \
    {                                                                                                                  \
        struct type *next; /* next element */                                                                          \
    }

#define RT_QUEUE_FIRST(head) ((head)->first)
#define RT_QUEUE_EMPTY(head) (RT_QUEUE_FIRST(head) == NULL)
#define RT_QUEUE_NEXT(elm, field) ((elm)->field.next)

#define RT_QUEUE_INIT(head)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        RT_QUEUE_FIRST(head) = NULL;                                                                                   \
        (head)->last = &RT_QUEUE_FIRST(head);                                                                          \
    } while (0)

#define RT_QUEUE_ENQUEUE(head, elm, field)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        RT_QUEUE_NEXT((elm), field) = NULL;                                                                            \
        *(head)->last = (elm);                                                                                         \
        (head)->last = &RT_QUEUE_NEXT((elm), field);                                                                   \
    } while (0)

#define RT_QUEUE_DEQUEUE(head, field)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((RT_QUEUE_FIRST((head)) = RT_QUEUE_NEXT(RT_QUEUE_FIRST((head)), field)) == NULL)                           \
        {                                                                                                              \
            (head)->last = &RT_QUEUE_FIRST((head));                                                                    \
        }                                                                                                              \
    } while (0)

#endif // RAY_TRACING_ONE_WEEK_RT_QUEUE_H
