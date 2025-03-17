/**
 * @file EVE_Platform_ESP32.h
 * @brief Eve_Hal framework APIs for ESP32 host platform
 *
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 *
 * @date 2024
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

#ifndef EVE_PLATFORM_ESP32__H
#define EVE_PLATFORM_ESP32__H

#include "EVE_Config.h"
/* Standard C libraries */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

#if defined(ESP32_PLATFORM)

/* ESP32 platform libraries */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

/** @name ESP32 default platform definitions.
 * @warning Configuration can be changed programatically in initialization parameters.
 * Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments */
///@{
#define EVE_SPI SPI2_HOST
#define EVE_DEFAULT_SPI_PD 6  /**< GPIO6 */
#define EVE_DEFAULT_SPI_CS 10 /**< GPIO10 */

// #define EVE_DEFAULT_SPI_MOSI   35 /**< GPIO35 */
// #define EVE_DEFAULT_SPI_SCK    36 /**< GPIO36 */
// #define EVE_DEFAULT_SPI_MISO   37 /**< GPIO37 */

// demo board
#define EVE_DEFAULT_SPI_MOSI 13 /**< GPIO13 */
#define EVE_DEFAULT_SPI_SCK 12  /**< GPIO12 */
#define EVE_DEFAULT_SPI_MISO 11 /**< GPIO11 */

///@}
#endif
#endif /* #ifndef EVE_PLATFORM_ESP32__H */

/* end of file */
