#ifndef __BSP_MATH_H
#define __BSP_MATH_H

#define PI 3.14159265f

#include "stdint.h"

#define MATH_ABS(x) ((x) > 0 ? (x) : -(x))

#define ABS_LIMIT(a, ABS_MAX)  \
    do                         \
    {                          \
        if (*(a) > (ABS_MAX))  \
        {                      \
            *(a) = (ABS_MAX);  \
        }                      \
        if (*(a) < -(ABS_MAX)) \
        {                      \
            *(a) = -(ABS_MAX); \
        }                      \
    } while (0)

void math_endian_reverse_16(void *source, void *destination);

#endif