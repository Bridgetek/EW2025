/**
 * @file Scanout.h
 * @brief Swapchain configuration and management functions
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef SWAPCHAIN_H_
#define SWAPCHAIN_H_

#include <stdio.h>
#include <stdint.h>
#include "EVE_CoCmd.h"

#include "EW2024_Photobooth_Utils.h"

/** Swapchain parameters */
#define MIN_PTRS 2
#define MAX_PTRS 4

/** Struct to store pointers */
typedef struct
{
    uint32_t ptrs[MAX_PTRS];         // address of a buffer
    uint32_t eve_reg_ptrs[MAX_PTRS]; // address of EVE registers PTR
    uint32_t eve_reg_reset;          // address of EVE registers RESET
    uint32_t buffer_size[MAX_PTRS];  // size of a buffer
    uint8_t enable;
    uint8_t num_ptrs; // Number of non-null SCs, range 2-4

    uint32_t eve_swapchain; // [SWAPCHAIN_0, SWAPCHAIN_1, SWAPCHAIN_2]
    uint32_t eve_reg_size;

    // for debugger
    char name[100];

} Swapchain_t;

extern Swapchain_t Swapchain[3];

/** Shortcut to Swapchain */
#define Swapchain0 (Swapchain[0])
#define Swapchain1 (Swapchain[1])
#define Swapchain2 (Swapchain[2])

#define Swapchain_Render_Engine Swapchain0
#define Swapchain_Video_Engine Swapchain1
#define Swapchain_Lvds_Input Swapchain2

#define SC_NUM 3

#define SC_Render_Engine_enable SC_0_enable
#define SC_Video_Engine_enable SC_1_enable
#define SC_Lvds_Input_enable SC_2_enable

#define REG_SC_Render_Engine_SIZE REG_SC0_SIZE
#define REG_SC_Render_Engine_PTR0 REG_SC0_PTR0
#define REG_SC_Render_Engine_PTR1 REG_SC0_PTR1
#define REG_SC_Render_Engine_PTR2 REG_SC0_PTR2
#define REG_SC_Render_Engine_PTR3 REG_SC0_PTR3

#define REG_SC_Video_Engine_SIZE REG_SC1_SIZE
#define REG_SC_Video_Engine_PTR0 REG_SC1_PTR0
#define REG_SC_Video_Engine_PTR1 REG_SC1_PTR1
#define REG_SC_Video_Engine_PTR2 REG_SC1_PTR2
#define REG_SC_Video_Engine_PTR3 REG_SC1_PTR3

#define REG_SC_Lvds_Input_SIZE REG_SC2_SIZE
#define REG_SC_Lvds_Input_PTR0 REG_SC2_PTR0
#define REG_SC_Lvds_Input_PTR1 REG_SC2_PTR1
#define REG_SC_Lvds_Input_PTR2 REG_SC2_PTR2
#define REG_SC_Lvds_Input_PTR3 REG_SC2_PTR3

void utils_scanoutInit(Swapchain_t *sc, uint32_t eve_sc_id, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4);
void utils_scanoutInitBySize(Swapchain_t *sc, uint32_t buffer_size);
void utils_scanoutInitDefault();
void utils_scanoutQueryPtr();
void utils_scanoutSet();
void utils_scanout(Swapchain_t *sc, uint32_t address, uint32_t extsyncmode, uint32_t color_format);
void utils_scanoutDump();

#endif /* SWAPCHAIN_H_ */
