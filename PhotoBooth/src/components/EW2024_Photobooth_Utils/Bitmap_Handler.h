/**
 * @file Bitmap_Handler.h
 * @brief This file contains the interface for bitmap handler.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "EW2024_Photobooth_Utils.h"

#ifndef HANDLER_H_
#define HANDLER_H_

#define BMHL_AUTO 99 // auto handler, only a few handler available 0-15
#define BMHL_NONE 98 // no handler

// 16-34 for ROM font
// Vertex2II uses 0-31 handle only. This is different from BitmapHandle.
#define MAX_HANDLER 31
#define MIN_HANDLER 1
#define ROM_FONT_START 16
#define ROM_FONT_END 34

#define IS_BITMAP_HNDLER_VALID(val) ((val >= 0 && val <= MAX_HANDLER) || (val >= ROM_FONT_START && val <= ROM_FONT_END))

#define bitmap_handle_set(phost, val)    \
    {                                    \
        if (IS_BITMAP_HNDLER_VALID(val)) \
        {                                \
            bitmap_handle(phost, val);   \
        }                                \
    }

uint32_t utils_BitmapHandler_get();

#endif // HANDLER_H_
