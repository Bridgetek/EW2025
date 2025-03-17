/**
 * @file Ddr.h
 * @brief EVE DDR memory management functions.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef DDR_H_
#define DDR_H_

#define EVE_MEMORY_ALIGNMENT 4

typedef struct MemoryBlock
{
    uint32_t size;
    bool isFree;
    struct MemoryBlock *next;
} MemoryBlock;
void utils_ddrInit(uint32_t totalSize);
uint32_t utils_ddrAlloc(uint32_t size);
uint32_t utils_ddrAllocAlignment(uint32_t size, uint32_t alignment);

#endif // DDR_H_