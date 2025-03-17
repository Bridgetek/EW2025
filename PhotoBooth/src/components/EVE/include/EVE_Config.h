/**
 * @file EVE_Config.h
 * @brief This file processes the input definitions
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

#ifndef EVE_CONFIG__H
#define EVE_CONFIG__H

#include <stddef.h>
#include <wchar.h>

/* Include C99 headers */
#include <stdint.h>
#include <stdbool.h>

/** @name Chip IDs with EVE generation */
///@{
#define EVE_BT820 0x50820
///@}

/** @name EVE generations */
///@{
#define EVE5 5 /**< BT820 */
///@}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
/** @attention

The following values are set based on the input definitions.
Do not set these values manually; instead, use the specific user definitions.

Graphics target:
- EVE_SUPPORT_CHIPID=EVE_BT820

Platform target:
- ESP32_PLATFORM (set EVE_PLATFORM_ESP32)

Display resolution:
- DISPLAY_RESOLUTION_WUXGA

Additionally, the following support flags are set:
- EVE_SUPPORT_UNICODE
- EVE_SUPPORT_MEDIAFIFO
- EVE_SUPPORT_CAPACITIVE
- EVE_SUPPORT_RESISTIVE

*/

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
/** Validate the configured options. **/
#if defined(EVE_GRAPHICS_BT820)
#define EVE_GRAPHICS_AVAILABLE
#endif

#if defined(EVE_DISPLAY_WUXGA)
#define EVE_DISPLAY_AVAILABLE
#endif

#if defined(EVE_PLATFORM_ESP32)
#define EVE_PLATFORM_AVAILABLE
#endif

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

/** @details

The selected graphics module below must set one of the following options.
(Users must select the graphics module definition, *not* one of these enable options.)
- BT820_ENABLE

It may also set platform, display, and flash values if none are configured.

*/

/** graphics module config **/
#if defined(EVE_GRAPHICS_BT820)

#define BT820_ENABLE
#define ENABLE_SPI_QUAD
#define RESISTANCE_THRESHOLD (1800)

#ifndef EVE_DISPLAY_AVAILABLE
#define EVE_DISPLAY_AVAILABLE
#define DISPLAY_RESOLUTION_WUXGA
#endif

#endif

// Re-Mapping BT820 Series to BT82X
#if defined(BT820_ENABLE)
#define BT82X_ENABLE
#define EVE_SUPPORT_CHIPID EVE_BT820
#define EVE_SUPPORT_GEN EVE5
#endif

/** Display config **/
#if defined(EVE_DISPLAY_WUXGA)
#define DISPLAY_RESOLUTION_WUXGA
#endif

/** Flash config **/
#define EVE_FLASH_SIZE 16

/** Platform config **/
/*

Ultimately, the platform selection must set one of the following internal platform flags.
- ESP32_PLATFORM
These may only be set by one of the platform target definitions, and should not be set manually by the user.

*/

#if defined(EVE_PLATFORM_ESP32)
#define ESP32_PLATFORM
#define EMBEDDED_PLATFORM
#define EVE_HOST EVE_HOST_EMBEDDED
#endif

#define EVE_CONFIG__STR(x) #x
#define EVE_CONFIG_STR(x) EVE_CONFIG__STR(x)

#if defined(ESP32_PLATFORM)
#define EVE_PLATFORM_AVAILABLE
#endif

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define EVE_CHIPID EVE_SUPPORT_CHIPID
#define EVE_GEN EVE_SUPPORT_GEN

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

/** @name Feature support.
 * Avoid hardcoding specific EVE models throughout the libraries.
 * \n Allows disabling specific features for debugging purposes.
 */
///@{
#define EVE_SUPPORT_UNICODE
#define EVE_SUPPORT_MEDIAFIFO
#define EVE_SUPPORT_CAPACITIVE
///@}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Other options
#define EVE_CMD_HOOKS 0 /**< Allow adding a callback hook into EVE_CoCmd calls using CoCmdHook in EVE_HalContext */

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Platform special
#define eve_progmem
#define eve_progmem_const const

typedef eve_progmem int8_t eve_prog_int8_t;
typedef eve_progmem uint8_t eve_prog_uint8_t;
typedef eve_progmem uint16_t eve_prog_uint16_t;

#ifndef EVE_TCHAR_DEFINED
#define EVE_TCHAR_DEFINED
#ifdef _WIN32
typedef wchar_t eve_tchar_t;
#else
typedef char eve_tchar_t;
#endif
#endif

#ifndef _MSC_VER
/* strcpy_s is not available in GCC */
#define strcpy_s(dst, sz, src) strcpy(dst, src)
#endif

#if !defined(NDEBUG) && !defined(_DEBUG)
#define _DEBUG 1
#endif

#if defined(__GNUC__)
#define DO_PRAGMA_(x) _Pragma(#x)
#define eve_pragma_warning(msg) DO_PRAGMA_("GCC warning \"" msg "\"")
#else
#define eve_pragma_warning(msg)
#endif

#if defined(__GNUC__)
#define eve_pragma_error(msg) _Pragma("GCC error \"" msg "\"")
#else
#define eve_pragma_error(msg)
#endif

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

/// Configuration sanity checks
#if !defined(EVE_SUPPORT_CHIPID)
#pragma message(__FILE__ "(" EVE_CONFIG_STR(__LINE__) "): error EVE_SUPPORT_CHIPID: " \
                                                      "No EVE device model has been selected")
#endif
#if !defined(EVE_DISPLAY_AVAILABLE)
#pragma message(__FILE__ "(" EVE_CONFIG_STR(__LINE__) "): error EVE_DISPLAY_AVAILABLE: " \
                                                      "No display model has been selected")
#endif
#if !defined(EVE_PLATFORM_AVAILABLE)
#pragma message(__FILE__ "(" EVE_CONFIG_STR(__LINE__) "): warning PLATFORM: " \
                                                      "No platform was selected")
#endif

#endif /* EVE_CONFIG__H */

/* end of file */
