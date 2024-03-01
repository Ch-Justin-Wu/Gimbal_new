#include "bsp_math.h"

// 16位大小段转换
void math_endian_reverse_16(void *source, void *destination)
{
    uint8_t *temp_source = (uint8_t *)source;
    uint8_t *temp_destination = (uint8_t *)destination;

    temp_destination[0] = temp_source[1];
    temp_destination[1] = temp_source[0];
}

