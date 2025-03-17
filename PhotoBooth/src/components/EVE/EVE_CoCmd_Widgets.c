/**
 * @file EVE_CoCmd_Widgets.c
 * @brief EVE's co-processor command
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

#include "EVE_Platform.h"

#include <stdarg.h>

/** @brief Count number of arguments in Cmd_Text for string format
 *
 * @param str
 * @return count
 */
static uint8_t countArgs(const char *str)
{
	uint8_t count = 0;
	const char *tmp = str;

	while ((tmp = strstr(tmp, "%")))
	{
		if (*(tmp + 1) == '%')
		{
			tmp += 2;
		}
		else
		{
			count++;
			tmp++;
		}
	}
	return count;
}

void EVE_CoCmd_text(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, const char *s, ...)
{
	va_list args;
	uint8_t num;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_TEXT, 0))
		return;
#endif

	if (font >= 64)
	{
		eve_printf_debug("Invalid font handle specified: %i\n", (int)font);
		return;
	}
	va_start(args, s);
	num = (options & OPT_FORMAT) ? (countArgs(s)) : (0); /* Only check % characters if option OPT_FORMAT is set */

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_TEXT);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, font);
	EVE_Cmd_wr16(phost, options);
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	for (uint8_t i = 0; i < num; i++)
		EVE_Cmd_wr32(phost, (uint32_t)va_arg(args, uint32_t));
	EVE_Cmd_endFunc(phost);
	va_end(args);
}

void EVE_CoCmd_text_s(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, const char *s, uint32_t length)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_TEXT, 0))
		return;
#endif

	if (font >= 64)
	{
		eve_printf_debug("Invalid font handle specified: %i\n", (int)font);
		return;
	}

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_TEXT);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, font);
	EVE_Cmd_wr16(phost, options);
	EVE_Cmd_wrString(phost, s, length);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_text_ex(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, bool bottom, int16_t baseLine, int16_t capsHeight, int16_t xOffset, const char *s)
{
	int16_t yOffset;
	if (options & OPT_CENTERY)
		yOffset = baseLine - (capsHeight >> 1);
	else if (bottom)
		yOffset = baseLine;
	else
		yOffset = baseLine - capsHeight;
	/*
	if (options & OPT_RIGHTX)
		xOffset = 0;
	else if (options & OPT_CENTERX)
		xOffset >>= 1;
	*/
	EVE_CoCmd_text(phost, x - xOffset, y - yOffset, font, options & ~OPT_CENTERY, s);
}

void EVE_CoCmd_button(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const char *s, ...)
{
	va_list args;
	uint8_t num;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_BUTTON, 0))
		return;
#endif

	if (font >= 64)
	{
		eve_printf_debug("Invalid font handle specified: %i\n", (int)font);
		return;
	}

	va_start(args, s);
	num = (options & OPT_FORMAT) ? (countArgs(s)) : (0); /* Only check % characters if option OPT_FORMAT is set */
	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_BUTTON);
	EVE_Cmd_wr32(phost, (((uint32_t)y << 16) | (x & 0xffff)));
	EVE_Cmd_wr32(phost, (((uint32_t)h << 16) | (w & 0xffff)));
	EVE_Cmd_wr32(phost, (((uint32_t)options << 16) | (font & 0xffff)));
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	for (uint8_t i = 0; i < num; i++)
		EVE_Cmd_wr32(phost, (uint32_t)va_arg(args, uint32_t));
	EVE_Cmd_endFunc(phost);
	va_end(args);
}

void EVE_CoCmd_keys(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const char *s)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_KEYS, 0))
		return;
#endif

	if (font >= 64)
	{
		eve_printf_debug("Invalid font handle specified: %i\n", (int)font);
		return;
	}

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_KEYS);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, w);
	EVE_Cmd_wr16(phost, h);
	EVE_Cmd_wr16(phost, font);
	EVE_Cmd_wr16(phost, options);
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_toggle(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t font, uint16_t options, uint16_t state, const char *s, ...)
{
	va_list args;
	uint8_t num;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_TOGGLE, 0))
		return;
#endif

	if (font >= 64)
	{
		eve_printf_debug("Invalid font handle specified: %i\n", (int)font);
		return;
	}

	va_start(args, s);
	num = (options & OPT_FORMAT) ? (countArgs(s)) : (0); // Only check % characters if option OPT_FORMAT is set

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_TOGGLE);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, w);
	EVE_Cmd_wr16(phost, font);
	EVE_Cmd_wr16(phost, options);
	EVE_Cmd_wr16(phost, state);
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	for (uint8_t i = 0; i < num; i++)
		EVE_Cmd_wr32(phost, (uint32_t)va_arg(args, uint32_t));
	EVE_Cmd_endFunc(phost);
	va_end(args);
}

void EVE_CoCmd_number(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, int32_t n)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_NUMBER, 0))
		return;
#endif

	if (font >= 64)
	{
		eve_printf_debug("Invalid font handle specified: %i\n", (int)font);
		return;
	}

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_NUMBER);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, font);
	EVE_Cmd_wr16(phost, options);
	EVE_Cmd_wr32(phost, n);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_progress(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range)
{
	EVE_CoCmd_dwwwwwww(phost, CMD_PROGRESS, x, y, w, h, options, val, range);
}

void EVE_CoCmd_slider(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range)
{
	EVE_CoCmd_dwwwwwww(phost, CMD_SLIDER, x, y, w, h, options, val, range);
}

void EVE_CoCmd_scrollbar(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t size, uint16_t range)
{
	EVE_CoCmd_dwwwwwwww(phost, CMD_SCROLLBAR, x, y, w, h, options, val, size, range);
}

void EVE_CoCmd_gauge(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range)
{
	EVE_CoCmd_dwwwwwwww(phost, CMD_GAUGE, x, y, r, options, major, minor, val, range);
}

void EVE_CoCmd_clock(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms)
{
	EVE_CoCmd_dwwwwwwww(phost, CMD_CLOCK, x, y, r, options, h, m, s, ms);
}

void EVE_CoCmd_dial(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t val)
{
	EVE_CoCmd_dwwwww(phost, CMD_DIAL, x, y, r, options, val);
}

void EVE_CoCmd_arc(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r0, uint16_t r1, uint16_t a0, uint16_t a1)
{
	EVE_CoCmd_dwwwwww(phost, CMD_ARC, x, y, r0, r1, a0, a1);
}

void EVE_CoCmd_gradient(EVE_HalContext *phost, int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1)
{
	EVE_CoCmd_dwwdwwd(phost, CMD_GRADIENT, x0, y0, rgb0, x1, y1, rgb1);
}

void EVE_CoCmd_gradientA(EVE_HalContext *phost, int16_t x0, int16_t y0, uint32_t argb0, int16_t x1, int16_t y1, uint32_t argb1)
{
	EVE_CoCmd_dwwdwwd(phost, CMD_GRADIENTA, x0, y0, argb0, x1, y1, argb1);
}

void EVE_CoCmd_cgradient(EVE_HalContext *phost, uint32_t shape, int16_t x, int16_t y, int16_t w, uint16_t h, uint32_t argb0, uint32_t argb1)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_CGRADIENT, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_CGRADIENT);
	EVE_Cmd_wr32(phost, shape);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, w);
	EVE_Cmd_wr16(phost, h);
	EVE_Cmd_wr32(phost, argb0);
	EVE_Cmd_wr32(phost, argb1);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_glow(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, uint16_t h)
{
	EVE_CoCmd_dwwww(phost, CMD_GLOW, x, y, w, h);
}

/* end of file */
