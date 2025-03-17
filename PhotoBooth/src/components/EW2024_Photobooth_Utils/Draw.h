/**
 * @file Draw.h
 * @brief Header file for drawing utilities used in the photobooth application.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef _DRAW_H_
#define _DRAW_H_

// Define the various cases
#define utils_drawBoxText_4(phost, x, y, w, h) utils_drawBoxText(phost, x, y, w, h, 2, 0, 0, 0, 30, 0, 0)
uint32_t utils_drawBoxText(EVE_HalContext *phost, uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                           uint32_t rounded, uint8_t *color, uint32_t tag, char *text, uint32_t font, uint32_t otp, uint8_t *tcolor);
uint32_t utils_drawBoxTextLeft(EVE_HalContext *phost, uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                               uint32_t rounded, uint8_t *color, uint32_t tag, char *text, uint32_t font, uint32_t otp, uint8_t *tcolor);

#endif // _DRAW_H_