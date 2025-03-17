/**
 * @file Bitmap_Handler.c
 * @brief Bitmap handler module to manage the bitmap handle in a BT820 EVE GUI Project.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "Bitmap_Handler.h"

// Bitmap handle. The initial value is 0. The valid value range is from 0 to 63.
static uint32_t bitmap_handler_inc = MIN_HANDLER;

uint32_t utils_BitmapHandler_get()
{
	if (bitmap_handler_inc > MAX_HANDLER)
	{
		return BMHL_NONE;
	}

	if (bitmap_handler_inc == ROM_FONT_START)
	{
		bitmap_handler_inc = ROM_FONT_END + 1;
	}

	bitmap_handler_inc++;

	return bitmap_handler_inc - 1;
}
