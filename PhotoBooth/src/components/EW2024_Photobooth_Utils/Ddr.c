/**
 * @file Ddr.c
 * @brief Dynamic memory allocation for EVE using DDR memory
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "Ddr.h"

// In the RAM_G address space, the BT82X allocates the top 2.5 megabytes (2621440 bytes) of DDR
static uint32_t ddr_offset = 2621440;
static uint32_t ddr_size = 128*1024*1024; // 128 MB as default

void utils_ddrInit(uint32_t totalSize)
{
    if (totalSize < EVE_MEMORY_ALIGNMENT)
    {
        // Handle error
        printf("ramg is too small\n");
        return;
    }
    ddr_size = totalSize;
}

uint32_t utils_ddrAllocAlignment(uint32_t size, uint32_t alignment)
{
    ddr_offset = (ddr_offset + alignment - 1) & ~(alignment - 1);
    return utils_ddrAlloc(size);
}

uint32_t utils_ddrAlloc(uint32_t size)
{
    if (size == 0)
        return ddr_offset;

    size = (size + EVE_MEMORY_ALIGNMENT - 1) & ~(EVE_MEMORY_ALIGNMENT - 1); // Align size to 4096 bytes

    uint32_t startaddr = ddr_offset;
    if (ddr_offset + size > ddr_size)
    {
        // Handle error
        printf("Not enough memory\n");
        return 0;
    }
    ddr_offset += size;
    return startaddr;
}