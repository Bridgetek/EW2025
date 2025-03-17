/**
 * @file Scanout.c
 * @brief Provides APIs for scanout a display list on the screen
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth_Utils.h"
#include "Scanout.h"

/** Array of Swapchains */
Swapchain_t Swapchain[3];

/** Screen size */
static const uint32_t w = 1920;
static const uint32_t h = 1200;
static const uint32_t num_swapchain = 3;          // Swapchain 0, 1, 2
static const uint32_t swapchain_buffer_count = 4; // ptr 0, 1, 2, 3
static const uint32_t swapchain_buffer_size = swapchain_buffer_count * w * h;

/** Address of LVDS rx */
uint32_t lvds_input_addr = 0;
uint32_t ramg_render_buffer_addr = 0;

// 0: 1 pixel single // 1: 2 pixel single // 2: 2 pixel dual // 3: 4 pixel dual
#define PIXEL_SINGLE_1 0
#define PIXEL_SINGLE_2 1
#define PIXEL_DUAL_1 2
#define PIXEL_DUAL_2 3

#define ENABLE_SWAPCHAIN 0 // 0 = buffer on ramg, 1 = buffer on swapchain

/**
 * @brief Function to initialize Swapchain and set the first two pointers
 *
 * @param sc Swapchain
 * @param s1 Size of pointer 1st
 * @param s2 Size of pointer 2nd
 */
void utils_scanoutInit(Swapchain_t *sc, uint32_t eve_sc_id, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4)
{
    switch (eve_sc_id)
    {
    case SWAPCHAIN_0:
        sc->eve_reg_ptrs[0] = REG_SC0_PTR0;
        sc->eve_reg_ptrs[1] = REG_SC0_PTR1;
        sc->eve_reg_ptrs[2] = REG_SC0_PTR2;
        sc->eve_reg_ptrs[3] = REG_SC0_PTR3;
        sc->eve_reg_reset = REG_SC0_RESET;
        sc->eve_swapchain = SWAPCHAIN_0;
        sprintf(sc->name, "SWAPCHAIN_0");
        break;
    case SWAPCHAIN_1:
        sc->eve_reg_ptrs[0] = REG_SC1_PTR0;
        sc->eve_reg_ptrs[1] = REG_SC1_PTR1;
        sc->eve_reg_ptrs[2] = REG_SC1_PTR2;
        sc->eve_reg_ptrs[3] = REG_SC1_PTR3;
        sc->eve_reg_reset = REG_SC1_RESET;
        sc->eve_reg_size = REG_SC1_SIZE;
        sc->eve_swapchain = SWAPCHAIN_1;
        sprintf(sc->name, "SWAPCHAIN_1");
        break;
    case SWAPCHAIN_2:
        sc->eve_reg_ptrs[0] = REG_SC2_PTR0;
        sc->eve_reg_ptrs[1] = REG_SC2_PTR1;
        sc->eve_reg_ptrs[2] = REG_SC2_PTR2;
        sc->eve_reg_ptrs[3] = REG_SC2_PTR3;
        sc->eve_reg_reset = REG_SC2_RESET;
        sc->eve_reg_size = REG_SC2_SIZE;
        sc->eve_swapchain = SWAPCHAIN_2;
        sprintf(sc->name, "SWAPCHAIN_2");
        break;
    default:
        sc->eve_reg_ptrs[0] = REG_SC0_PTR0;
        sc->eve_reg_ptrs[1] = REG_SC0_PTR1;
        sc->eve_reg_ptrs[2] = REG_SC0_PTR2;
        sc->eve_reg_ptrs[3] = REG_SC0_PTR3;
        sc->eve_reg_size = REG_SC0_SIZE;
        sc->eve_reg_reset = REG_SC0_RESET;
        sprintf(sc->name, "SWAPCHAIN_0");
        break;
    }

    sc->ptrs[0] = p1;
    sc->ptrs[1] = p2;
    sc->ptrs[2] = p3;
    sc->ptrs[3] = p4;

    sc->enable = 1;
    sc->num_ptrs = 4;
}

void utils_scanoutInitBySize(Swapchain_t *sc, uint32_t buffer_size)
{
    uint32_t eve_sc_id = SWAPCHAIN_0;

    if (sc == &Swapchain_Render_Engine)
    {
        eve_sc_id = SWAPCHAIN_0;
    }
    else if (sc == &Swapchain_Video_Engine)
    {
        eve_sc_id = SWAPCHAIN_1;
    }
    else if (sc == &Swapchain_Lvds_Input)
    {
        eve_sc_id = SWAPCHAIN_2;
    }

    sc->ptrs[0] = utils_ddrAllocAlignment(buffer_size, 128);
    sc->ptrs[1] = utils_ddrAllocAlignment(buffer_size, 128);
    sc->ptrs[2] = utils_ddrAllocAlignment(buffer_size, 128);
    sc->ptrs[3] = utils_ddrAllocAlignment(buffer_size, 128);

    sc->buffer_size[0] = buffer_size;
    sc->buffer_size[1] = buffer_size;
    sc->buffer_size[2] = buffer_size;
    sc->buffer_size[3] = buffer_size;

    utils_scanoutInit(sc, eve_sc_id, sc->ptrs[0], sc->ptrs[1], sc->ptrs[2], sc->ptrs[3]);
}

void utils_scanoutInit_size_default(Swapchain_t *sc)
{
    uint32_t w = PHOST->Width;
    uint32_t h = PHOST->Height;
    uint32_t sc_num_ptr = 4; // ptr 0, 1, 2, 3
    uint32_t sc_size = sc_num_ptr * w * h * 2;

    utils_scanoutInitBySize(sc, sc_size);
}

/**
 * @brief Connect an Swapchain to Scan Out
 *
 * @param sc Swapchain
 */
void utils_scanout(Swapchain_t *sc, uint32_t address, uint32_t extsyncmode, uint32_t color_format)
{
    wr32(PHOST, REG_SO_MODE, extsyncmode);
    wr32(PHOST, REG_SO_SOURCE, address);
    wr32(PHOST, REG_SO_FORMAT, color_format);
    wr32(PHOST, REG_SO_EN, 1);
    cmd_rendertarget(PHOST, address, color_format, 1920, 1200);
    EVE_Cmd_waitFlush(PHOST);
}

/**
 * @brief Write Swapchain configurations to EVE
 *
 */
void utils_scanoutSet()
{
    if (Swapchain_Lvds_Input.enable != 0)
    {
        uint32_t TXPLLDiv = 0x03;
        wr32(PHOST, REG_LVDS_PLLCFG, (TXPLLDiv > 4 ? (0x00300870 + TXPLLDiv) : (0x00301070 + TXPLLDiv)));
        wr32(PHOST, REG_LVDS_EN, 0);
        wr32(PHOST, REG_LVDS_EN, 7);
        EVE_sleep(10);
        APP_INF("LVDS is enabled\n");
    }

    // Iterate over the array using a for loop
    for (int i = 0; i < SC_NUM; i++)
    {
        Swapchain_t *sc = &Swapchain[i];
        if (sc->enable == 0)
        {
            continue;
        }

        wr32(PHOST, sc->eve_reg_size, sc->num_ptrs);

        // Iterate over the array using a for loop
        for (int j = 0; j < sc->num_ptrs; j++)
        {

            uint32_t ptr = sc->ptrs[j];
            uint32_t eve_ptr = sc->eve_reg_ptrs[j];
            cmd_regwrite(PHOST, eve_ptr, ptr);
        }
    }

    wr32(PHOST, REG_RX_SETUP, 1);

#if ENABLE_SWAPCHAIN
    wr32(PHOST, REG_RX_DEST, Swapchain_Lvds_Input.eve_swapchain);
#else
    wr32(PHOST, REG_RX_DEST, lvds_input_addr);
#endif

    wr32(PHOST, REG_RX_FORMAT, RGB8);
    wr32(PHOST, REG_RX_DITHER, 0);
    wr32(PHOST, REG_RX_CAPTURE, 1);
    wr32(PHOST, REG_RX_ENABLE, 1);
    wr32(PHOST, REG_LVDSRX_SYS_SETUP, REG_LVDSRX_SYS_SETUP_VALUE);
    wr32(PHOST, REG_LVDSRX_SYS_CTRL, REG_LVDSRX_SYS_CTRL_VALUE);
    EVE_Cmd_waitFlush(PHOST);

    utils_scanoutQueryPtr();
}

void utils_scanoutQueryPtr()
{
#if !ENABLE_SWAPCHAIN
    return;
#endif
    if (!Swapchain_Lvds_Input.enable)
    {
        return;
    }

    // # pull the LVDS data address
    uint32_t valid = 0;
    Timer timer;
    utils_timerStart(&timer, 1000, 0);
    while (!valid)
    {
        cmd_regread(PHOST, REG_SC2_STATUS, &valid);
        EVE_sleep(10);
        if (utils_timerIsExpired(&timer))
        {
            APP_INF("query lvds ptr timeout\n");
            break;
        }
    }
    cmd_regread(PHOST, REG_SC2_ADDR, &lvds_input_addr);
    EVE_Cmd_waitFlush(PHOST);
}

void utils_scanoutInit_use_swapchain()
{
    utils_scanoutInit_size_default(&Swapchain_Video_Engine);
    utils_scanoutInit_size_default(&Swapchain_Render_Engine);
    utils_scanoutInit_size_default(&Swapchain_Lvds_Input);

    utils_scanoutSet();
    finish(PHOST);
    utils_scanout(&Swapchain_Render_Engine, Swapchain_Render_Engine.eve_swapchain, PIXEL_DUAL_2, YCBCR);
}
void utils_scanoutInit_use_ramg()
{
    utils_scanoutInit_size_default(&Swapchain_Video_Engine);
    utils_scanoutInit_size_default(&Swapchain_Render_Engine);
    utils_scanoutInit_size_default(&Swapchain_Lvds_Input);

    wr32(PHOST, REG_SO_EN, 0);
    cmd_regwrite(PHOST, REG_RX_SETUP, 0);
    cmd_regwrite(PHOST, REG_RX_ENABLE, 0);
    EVE_Cmd_waitFlush(PHOST);

    if (lvds_input_addr == 0)
    {
        lvds_input_addr = utils_ddrAllocAlignment(swapchain_buffer_size, 128);
    }
    if (ramg_render_buffer_addr == 0)
    {
        ramg_render_buffer_addr = utils_ddrAllocAlignment(swapchain_buffer_size, 128);
    }

    utils_scanoutSet();
    utils_scanout(&Swapchain_Render_Engine, ramg_render_buffer_addr, PIXEL_DUAL_2, RGB8);
}

void utils_scanoutInitDefault()
{
#if ENABLE_SWAPCHAIN
    return utils_scanoutInit_use_swapchain();
#else
    return utils_scanoutInit_use_ramg();
#endif
}
