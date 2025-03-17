/**
 * @file EVE_Hal.h
 * @brief Main file to include the EVE HAL in your applications
 *
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 *
 * @date 2018
 *
 * MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EVE_HAL__H
#define EVE_HAL__H

/*************
** INCLUDES **
*************/
#include "EVE_Platform.h"

/**********************
** GLOBAL PROTOTYPES **
**********************/

/** @name INIT */
///@{
/** Initialize HAL platform */
EVE_HalPlatform *EVE_Hal_initialize();
/** Release HAL platform */
void EVE_Hal_release();
/** Get the default configuration parameters */
void EVE_Hal_defaults(EVE_HalParameters *parameters);
/** Get the default configuration parameters.
Use `deviceIdx` to choose the connected device, or set to -1 to get the first available device. */
void EVE_Hal_defaultsEx(EVE_HalParameters *parameters, size_t deviceIdx);
/** Opens a new HAL context using the specified parameters */
bool EVE_Hal_open(EVE_HalContext *phost, const EVE_HalParameters *parameters);
/** Close a HAL context */
void EVE_Hal_close(EVE_HalContext *phost);
/** Idle. Call regularly to update frequently changing internal state.
This is also called while waiting for cmd, in addition to the user idle callback */
void EVE_Hal_idle(EVE_HalContext *phost);
///@}

/** @name TRANSFER HELPERS */
///@{

uint8_t EVE_Hal_rd8(EVE_HalContext *phost, uint32_t addr);
uint16_t EVE_Hal_rd16(EVE_HalContext *phost, uint32_t addr);
uint32_t EVE_Hal_rd32(EVE_HalContext *phost, uint32_t addr);
void EVE_Hal_rdMem(EVE_HalContext *phost, uint8_t *result, uint32_t addr, uint32_t size);

void EVE_Hal_wr8(EVE_HalContext *phost, uint32_t addr, uint8_t v);
void EVE_Hal_wr16(EVE_HalContext *phost, uint32_t addr, uint16_t v);
void EVE_Hal_wr32(EVE_HalContext *phost, uint32_t addr, uint32_t v);
void EVE_Hal_wrMem(EVE_HalContext *phost, uint32_t addr, const uint8_t *buffer, uint32_t size);
void EVE_Hal_wrProgMem(EVE_HalContext *phost, uint32_t addr, eve_progmem_const uint8_t *buffer, uint32_t size);
void EVE_Hal_wrString(EVE_HalContext *phost, uint32_t addr, const char *str, uint32_t index, uint32_t size, uint32_t padMask);
///@}

/** @name SPI command */
///@{
void EVE_Hal_SPICmd_pwr_state(EVE_HalContext *phost, EVE_PWR_STATE_T state);
void EVE_Hal_SPICmd_setpllsp1(EVE_HalContext *phost, EVE_CMD_SYS_PLL_FREQ val);
void EVE_Hal_SPICmd_sysclkdiv(EVE_HalContext *phost, EVE_CMD_SYS_CLK_DIV val);
void EVE_Hal_SPICmd_setbootcfg(EVE_HalContext *phost, EVE_CMD_BOOT_CFG val);
void EVE_Hal_SPICmd_bootcfgen(EVE_HalContext *phost, EVE_CMD_BOOT_CFG_EN val);
void EVE_Hal_SPICmd_setddrtype(EVE_HalContext *phost, EVE_CMD_DDR_TYPE val);
void EVE_Hal_SPICmd_setgpreg(EVE_HalContext *phost, EVE_CMD_GPREG val);

void EVE_Host_clockSelect(EVE_HalContext *phost, EVE_PLL_SOURCE_T pllsource);
void EVE_Host_pllFreqSelect(EVE_HalContext *phost, EVE_PLL_FREQ_T freq);
void EVE_Host_powerModeSwitch(EVE_HalContext *phost, EVE_POWER_MODE_T pwrmode);
void EVE_Host_coreReset(EVE_HalContext *phost);
/** This API can only be called when PLL is stopped(SLEEP mode).
For compatibility, set frequency to the EVE_GPU_12MHZ option in the EVE_SETPLLSP1_T table. */
void EVE_Host_selectSysClk(EVE_HalContext *phost, EVE_82X_PLL_FREQ_T freq);
/* Power down or up ROMs and ADCs.*/
void EVE_Host_powerOffComponents(EVE_HalContext *phost, uint8_t val);
/* This API sets the current strength of supported GPIO/IO group(s) */
void EVE_Host_padDriveStrength(EVE_HalContext *phost, EVE_82X_GPIO_DRIVE_STRENGTH_T strength, EVE_82X_GPIO_GROUP_T group);
/* This API will hold the system reset active,
EVE_Host_resetRemoval() must be called to release the system reset. */
void EVE_Host_resetActive(EVE_HalContext *phost);
/* This API will release the system reset,
and the system will exit reset and behave as after POR,
settings done through SPI commands will not be affected. */
void EVE_Host_resetRemoval(EVE_HalContext *phost);
///@}

/** @name DEBUG */
///@{
/** Display a fullscreen debug message using TEXT8X8.
Uses the back of RAM_G. */
void EVE_Hal_displayMessage(EVE_HalContext *phost, const char *str, uint16_t size);
/** Display a fullscreen debug message using TEXT8X8.
Uses the back of RAM_G. */
#define EVE_Hal_displayMessageZ(phost, str) EVE_Hal_displayMessage((phost), (str), (uint16_t)strlen((str)))
///@}
#endif /* #ifndef EVE_HAL__H */

/* end of file */
