/**
 * @file EVE_HalImpl.h
 * @brief Eve_Hal framework APIs
 *
 * This file defines the generic APIs of phost access layer for the FT800 or EVE compatible silicon.
 * Application shall access FT800 or EVE resources over these APIs,regardless of I2C or SPI protocol.
 * In addition, there are some helper functions defined for FT800 coprocessor engine as well as phost commands.
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

#ifndef EVE_HAL_IMPL__H
#define EVE_HAL_IMPL__H

/*********************
 *      INCLUDES
 *********************/
#include "EVE_HalDefs.h"

/**********************
 *  EXTERN VARIABLES
 **********************/
extern EVE_HalPlatform g_HalPlatform;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/** @name INIT */
///@{

/** Initialize HAL platform */
void EVE_HalImpl_initialize();
/** Release HAL platform */
void EVE_HalImpl_release();
/** List the available devices */
size_t EVE_Hal_list();
/** Get info of the specified device. Devices of type EVE_HOST_UNKNOWN should be ignored */
void EVE_Hal_info(EVE_DeviceInfo *deviceInfo, size_t deviceIdx);
/** Check whether the context is the specified device */
bool EVE_Hal_isDevice(EVE_HalContext *phost, size_t deviceIdx);
/** Get the default configuration parameters.
Use `deviceIdx` to choose the connected device. */
bool EVE_HalImpl_defaults(EVE_HalParameters *parameters, size_t deviceIdx);
/** Opens a new HAL context using the specified parameters */
bool EVE_HalImpl_open(EVE_HalContext *phost, const EVE_HalParameters *parameters);
/** Close a HAL context */
void EVE_HalImpl_close(EVE_HalContext *phost);
/** Idle. Call regularly to update frequently changing internal state */
void EVE_HalImpl_idle(EVE_HalContext *phost);
///@}

/** @name TRANSFER */
///@{

void EVE_Hal_startTransfer(EVE_HalContext *phost, EVE_TRANSFER_T rw, uint32_t addr);
void EVE_Hal_endTransfer(EVE_HalContext *phost);
uint8_t EVE_Hal_transfer8(EVE_HalContext *phost, uint8_t value);
uint16_t EVE_Hal_transfer16(EVE_HalContext *phost, uint16_t value);
uint32_t EVE_Hal_transfer32(EVE_HalContext *phost, uint32_t value);
void EVE_Hal_transferMem(EVE_HalContext *phost, uint8_t *result, const uint8_t *buffer, uint32_t size);
void EVE_Hal_transferProgMem(EVE_HalContext *phost, uint8_t *result, eve_progmem_const uint8_t *buffer, uint32_t size);
uint32_t EVE_Hal_transferString(EVE_HalContext *phost, const char *str, uint32_t index, uint32_t size, uint32_t padMask);
void EVE_Hal_flush(EVE_HalContext *phost);
///@}

/** @name UTILITY */
///@{
/** This API sends a SPI command to the phost */
void EVE_Hal_spiCommand(EVE_HalContext *phost, uint8_t *cmd);
/** Toggle PD_N pin of FT800 board for a power cycle. Returns false on failure */
bool EVE_Hal_powerCycle(EVE_HalContext *phost, bool up);
/** Switch EVE to different SPI channel mode */
void EVE_Hal_setSPI(EVE_HalContext *phost, EVE_SPI_CHANNELS_T numchnls);
/** Restore platform to previously configured EVE SPI channel mode */
void EVE_Hal_restoreSPI(EVE_HalContext *phost);
uint32_t EVE_Hal_currentFrequency(EVE_HalContext *phost);
///@}

/** @name MISC */
///@{

void EVE_Mcu_initialize();
void EVE_Mcu_release();

void EVE_Millis_initialize();
void EVE_Millis_release();
uint32_t EVE_millis();
uint64_t EVE_millis64();
void EVE_sleep(uint32_t ms);

bool EVE_UtilImpl_bootupDisplayGpio(EVE_HalContext *phost);
///@}

#endif /* #ifndef EVE_HAL_IMPL__H */

/* end of file */
