/**
 * @file EVE_CoCmd.c
 * @brief EVE's co-processor commmands
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

#include "EVE_Platform.h"

#if defined(_MSC_VER) && (_MSC_VER < 1800)
#include <stdio.h> /* memset */
#endif

/**********************************************************************
***********************************************************************
**********************************************************************/
void EVE_CoCmd_d(EVE_HalContext *phost, uint32_t cmd)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, 0))
		return;
#endif

	EVE_Cmd_wr32(phost, cmd);
}

void EVE_CoCmd_dd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_ddd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, uint32_t d1)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_wr32(phost, d1);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dddd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, uint32_t d1, uint32_t d2)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_wr32(phost, d1);
	EVE_Cmd_wr32(phost, d2);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_ddwww(EVE_HalContext *phost, uint32_t cmd,
					 uint32_t d0, uint16_t w1, uint16_t w2, uint16_t w3)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_wr16(phost, w1);
	EVE_Cmd_wr16(phost, w2);
	EVE_Cmd_wr16(phost, w3);
	EVE_Cmd_wr16(phost, 0);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dwwdwwd(EVE_HalContext *phost, uint32_t cmd,
					   uint16_t w0, uint16_t w1, uint32_t d2,
					   uint16_t w3, uint16_t w4, uint32_t d5)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr16(phost, w0);
	EVE_Cmd_wr16(phost, w1);
	EVE_Cmd_wr32(phost, d2);
	EVE_Cmd_wr16(phost, w3);
	EVE_Cmd_wr16(phost, w4);
	EVE_Cmd_wr32(phost, d5);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dwwww(EVE_HalContext *phost, uint32_t cmd,
					 uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr16(phost, w0);
	EVE_Cmd_wr16(phost, w1);
	EVE_Cmd_wr16(phost, w2);
	EVE_Cmd_wr16(phost, w3);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dwwwww(EVE_HalContext *phost, uint32_t cmd,
					  uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr16(phost, w0);
	EVE_Cmd_wr16(phost, w1);
	EVE_Cmd_wr16(phost, w2);
	EVE_Cmd_wr16(phost, w3);
	EVE_Cmd_wr16(phost, w4);
	EVE_Cmd_wr16(phost, 0);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dwwwwww(EVE_HalContext *phost, uint32_t cmd,
					   uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4, uint16_t w5)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr16(phost, w0);
	EVE_Cmd_wr16(phost, w1);
	EVE_Cmd_wr16(phost, w2);
	EVE_Cmd_wr16(phost, w3);
	EVE_Cmd_wr16(phost, w4);
	EVE_Cmd_wr16(phost, w5);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dwwwwwww(EVE_HalContext *phost, uint32_t cmd,
						uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3,
						uint16_t w4, uint16_t w5, uint16_t w6)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr16(phost, w0);
	EVE_Cmd_wr16(phost, w1);
	EVE_Cmd_wr16(phost, w2);
	EVE_Cmd_wr16(phost, w3);
	EVE_Cmd_wr16(phost, w4);
	EVE_Cmd_wr16(phost, w5);
	EVE_Cmd_wr16(phost, w6);
	EVE_Cmd_wr16(phost, 0);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dwwwwwwww(EVE_HalContext *phost, uint32_t cmd,
						 uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3,
						 uint16_t w4, uint16_t w5, uint16_t w6, uint16_t w7)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr16(phost, w0);
	EVE_Cmd_wr16(phost, w1);
	EVE_Cmd_wr16(phost, w2);
	EVE_Cmd_wr16(phost, w3);
	EVE_Cmd_wr16(phost, w4);
	EVE_Cmd_wr16(phost, w5);
	EVE_Cmd_wr16(phost, w6);
	EVE_Cmd_wr16(phost, w7);
	EVE_Cmd_endFunc(phost);
}

/**********************************************************************
***********************************************************************
**********************************************************************/
void EVE_CoCmd_dlStart(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_DLSTART);
}

void EVE_CoCmd_swap(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_SWAP);
}

void EVE_CoCmd_interrupt(EVE_HalContext *phost, uint32_t ms)
{
	EVE_CoCmd_dd(phost, CMD_INTERRUPT, ms);
}

void EVE_CoCmd_coldStart(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_COLDSTART);
	phost->CoScratchHandle = 15;
}

void EVE_CoCmd_sync(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_SYNC);
}

void EVE_CoCmd_clearCache(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_CLEARCACHE);
}

void EVE_CoCmd_nop(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_NOP);
}

void EVE_CoCmd_wait(EVE_HalContext *phost, uint32_t us)
{
	EVE_CoCmd_dd(phost, CMD_WAIT, us);
}

void EVE_CoCmd_return(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_RETURN);
}

void EVE_CoCmd_callList(EVE_HalContext *phost, uint32_t a)
{
	EVE_CoCmd_dd(phost, CMD_CALLLIST, a);
}

void EVE_CoCmd_newList(EVE_HalContext *phost, uint32_t a)
{
	EVE_CoCmd_dd(phost, CMD_NEWLIST, a);
}

void EVE_CoCmd_endList(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_ENDLIST);
}

bool EVE_CoCmd_getMatrix(EVE_HalContext *phost, int32_t *m)
{
	uint16_t resAddr;
	int i;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_GETMATRIX, 0))
		return false;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_GETMATRIX);
	resAddr = EVE_Cmd_moveWp(phost, 6 * 4);
	EVE_Cmd_endFunc(phost);

	/* Read result */
	if (m)
	{
		if (!EVE_Cmd_waitFlush(phost))
			return false;
		EVE_Hal_startTransfer(phost, EVE_TRANSFER_READ, RAM_CMD + resAddr);
		for (i = 0; i < 6; ++i)
			m[i] = EVE_Hal_transfer32(phost, 0);
		EVE_Hal_endTransfer(phost);
	}
	return true;
}

void EVE_CoCmd_setRotate(EVE_HalContext *phost, uint32_t r)
{
	const bool swapXY = r & 0x2;

	/* Update cached width and height based on rotation */
	if (swapXY)
	{
		phost->Width = EVE_Hal_rd16(phost, REG_VSIZE);
		phost->Height = EVE_Hal_rd16(phost, REG_HSIZE);
	}
	else
	{
		phost->Width = EVE_Hal_rd16(phost, REG_HSIZE);
		phost->Height = EVE_Hal_rd16(phost, REG_VSIZE);
	}

	/* Command */
	EVE_CoCmd_dd(phost, CMD_SETROTATE, r);

	/* Don't keep this in the write buffer */
	EVE_Hal_flush(phost);
}

void EVE_CoCmd_watchdog(EVE_HalContext *phost, uint32_t init_val)
{
	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_WATCHDOG);
	EVE_Cmd_wr32(phost, init_val);
	EVE_Cmd_endFunc(phost);

	EVE_Hal_flush(phost);
}
/**********************************************************************
***********************************************************************
**********************************************************************/
uint32_t EVE_CoCmd_calibrate(EVE_HalContext *phost, uint32_t result)
{
	uint32_t wp;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_CALIBRATE, 0))
		return 0;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_CALIBRATE);
	EVE_Cmd_wr32(phost, result);
	EVE_Cmd_endFunc(phost);

	/* Wait for the result */
	if (!EVE_Cmd_waitFlush(phost))
		return 0;
	wp = EVE_Hal_rd32(phost, REG_CMD_WRITE) & EVE_CMD_FIFO_MASK;
	return EVE_Hal_rd32(phost, RAM_CMD + ((CMDBUF_SIZE - 4) & (wp - 4)));
}

uint32_t EVE_CoCmd_calibrateSub(EVE_HalContext *phost, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t result)
{
	uint32_t wp;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_CALIBRATESUB, 0))
		return 0;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_CALIBRATESUB);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, w);
	EVE_Cmd_wr16(phost, h);
	EVE_Cmd_wr32(phost, result);
	EVE_Cmd_endFunc(phost);

	/* Wait for the result */
	if (!EVE_Cmd_waitFlush(phost))
		return 0;
	wp = EVE_Hal_rd32(phost, REG_CMD_WRITE) & EVE_CMD_FIFO_MASK;
	return EVE_Hal_rd32(phost, RAM_CMD + ((CMDBUF_SIZE - 4) & (wp - 4)));
}
/**********************************************************************
***********************************************************************
**********************************************************************/

void EVE_CoCmd_spinner(EVE_HalContext *phost, int16_t x, int16_t y, uint16_t style, uint16_t scale)
{
	EVE_CoCmd_dwwww(phost, CMD_SPINNER, x, y, style, scale);
}

void EVE_CoCmd_stop(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_STOP);
}

void EVE_CoCmd_track(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, int16_t tag)
{
	EVE_CoCmd_dwwwww(phost, CMD_TRACK, x, y, w, h, tag);
}

void EVE_CoCmd_screenSaver(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_SCREENSAVER);
}

void EVE_CoCmd_sketch(EVE_HalContext *phost, int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t ptr, uint16_t format)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_SKETCH, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_SKETCH);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr16(phost, w);
	EVE_Cmd_wr16(phost, h);
	EVE_Cmd_wr32(phost, ptr);
	EVE_Cmd_wr16(phost, format);
	EVE_Cmd_wr16(phost, 0);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_logo(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_LOGO);
}

void EVE_CoCmd_testCard(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_TESTCARD);
}
void EVE_CoCmd_renderTarget(EVE_HalContext *phost, uint32_t dst, uint16_t fmt, int16_t w, int16_t h)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_RENDERTARGET, 0))
		return;
#endif
	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_RENDERTARGET);
	EVE_Cmd_wr32(phost, dst);
	EVE_Cmd_wr16(phost, fmt);
	EVE_Cmd_wr16(phost, w);
	EVE_Cmd_wr16(phost, h);
	EVE_Cmd_wr16(phost, 0);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dsd(EVE_HalContext *phost, uint32_t cmd, const char *s, uint32_t d0)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_endFunc(phost);
}
void EVE_CoCmd_ddsd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, const char *s, uint32_t d1)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	EVE_Cmd_wr32(phost, d1);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_dds(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, const char *s)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	EVE_Cmd_endFunc(phost);
}
void EVE_CoCmd_ddds(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, uint32_t d1, const char *s)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, cmd, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, cmd);
	EVE_Cmd_wr32(phost, d0);
	EVE_Cmd_wr32(phost, d1);
	EVE_Cmd_wrString(phost, s, EVE_CMD_STRING_MAX);
	EVE_Cmd_endFunc(phost);
}
/* Nothing beyond this */
