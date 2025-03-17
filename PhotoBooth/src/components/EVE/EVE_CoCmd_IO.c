/**
 * @file EVE_CoCmd_IO.c
 * @brief EVE's co-processor IO commands
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

#define READ_4_BYTE_RESULT(result, phost) \
	;                                     \
	do                                    \
	{                                     \
		/* Wait for the result */         \
		if (!EVE_Cmd_waitFlush(phost))    \
			return false;                 \
		if (result)                       \
		{                                 \
			*result = previous(phost, 1); \
		}                                 \
		return true;                      \
	} while (0)

uint32_t previous(EVE_HalContext *phost, uint32_t offset)
{
	// """ offset 1 returns the last slot in the previous command """
	assert(0 < offset);
	uint32_t wp = EVE_Hal_rd32(phost, REG_CMD_WRITE) & EVE_CMD_FIFO_MASK;
	;
	return EVE_Hal_rd32(phost, RAM_CMD + ((CMDBUF_SIZE - 4) & (wp - 4 * offset)));
}

void EVE_CoCmd_memWrite(EVE_HalContext *phost, uint32_t ptr, uint32_t num)
{
	EVE_CoCmd_ddd(phost, CMD_MEMWRITE, ptr, num);
}

void EVE_CoCmd_memWrite32(EVE_HalContext *phost, uint32_t ptr, uint32_t value)
{
	EVE_CoCmd_dddd(phost, CMD_MEMWRITE, ptr, 4, value);
}

void EVE_CoCmd_memSet(EVE_HalContext *phost, uint32_t ptr, uint32_t value, uint32_t num)
{
	EVE_CoCmd_dddd(phost, CMD_MEMSET, ptr, value, num);
}

void EVE_CoCmd_memZero(EVE_HalContext *phost, uint32_t ptr, uint32_t num)
{
	EVE_CoCmd_ddd(phost, CMD_MEMZERO, ptr, num);
}

void EVE_CoCmd_memCpy(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num)
{
	EVE_CoCmd_dddd(phost, CMD_MEMCPY, dest, src, num);
}

void EVE_CoCmd_append(EVE_HalContext *phost, uint32_t ptr, uint32_t num)
{
	EVE_CoCmd_ddd(phost, CMD_APPEND, ptr, num);
}

void EVE_CoCmd_snapshot(EVE_HalContext *phost, uint32_t ptr)
{
	EVE_CoCmd_dd(phost, CMD_SNAPSHOT, ptr);
}

void EVE_CoCmd_inflate(EVE_HalContext *phost, uint32_t ptr, uint32_t options)
{
	EVE_CoCmd_ddd(phost, CMD_INFLATE, ptr, options);
}

void EVE_CoCmd_loadImage(EVE_HalContext *phost, uint32_t ptr, uint32_t options)
{
	EVE_CoCmd_ddd(phost, CMD_LOADIMAGE, ptr, options);
}

void EVE_CoCmd_mediaFifo(EVE_HalContext *phost, uint32_t ptr, uint32_t size)
{
	EVE_CoCmd_ddd(phost, CMD_MEDIAFIFO, ptr, size);
}

void EVE_CoCmd_videoStart(EVE_HalContext *phost, uint32_t options)
{
	EVE_CoCmd_dd(phost, CMD_VIDEOSTART, options);
}

void EVE_CoCmd_videoFrame(EVE_HalContext *phost, uint32_t dst, uint32_t ptr)
{
	EVE_CoCmd_dddd(phost, CMD_MEMWRITE, 3182934, 1, OPT_NODL); // WORKAROUND CMD_VIDEOFRAME
	EVE_CoCmd_ddd(phost, CMD_VIDEOFRAME, dst, ptr);
}

void EVE_CoCmd_playVideo(EVE_HalContext *phost, uint32_t options)
{
	EVE_CoCmd_dd(phost, CMD_PLAYVIDEO, options);

	if (options & (OPT_FLASH | OPT_MEDIAFIFO))
	{
		/* WORKAROUND: CMD_PLAYVIDEO completes immediately,
		but the command following it does not.
		Write a CMD_NOP command to behave as documented */
		EVE_CoCmd_nop(phost);
	}
}

void EVE_CoCmd_flashErase(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_FLASHERASE);
}

bool EVE_CoCmd_flashErase_flush(EVE_HalContext *phost)
{
	if (!EVE_Cmd_waitFlush(phost))
		return false; // Coprocessor must be ready

	EVE_CoCmd_d(phost, CMD_FLASHERASE);

	return EVE_Cmd_waitFlush(phost);
}

void EVE_CoCmd_flashWrite(EVE_HalContext *phost, uint32_t ptr, uint32_t num)
{
	EVE_CoCmd_ddd(phost, CMD_FLASHWRITE, ptr, num);
}

void EVE_CoCmd_flashRead(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num)
{
	EVE_CoCmd_dddd(phost, CMD_FLASHREAD, dest, src, num);
}

bool EVE_CoCmd_flashRead_flush(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num)
{
	if (!EVE_Cmd_waitFlush(phost))
		return false; // Coprocessor must be ready

	EVE_CoCmd_dddd(phost, CMD_FLASHREAD, dest, src, num);

	return EVE_Cmd_waitFlush(phost);
}

void EVE_CoCmd_flashUpdate(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num)
{
	EVE_CoCmd_dddd(phost, CMD_FLASHUPDATE, dest, src, num);
}

void EVE_CoCmd_flashDetach(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_FLASHDETACH);
}

uint32_t EVE_CoCmd_flashAttach(EVE_HalContext *phost)
{
	uint32_t flashStatus;
	if (!EVE_Cmd_waitFlush(phost))
		return EVE_Hal_rd32(phost, REG_FLASH_STATUS); // Coprocessor must be ready
	flashStatus = EVE_Hal_rd32(phost, REG_FLASH_STATUS);
	if (flashStatus != FLASH_STATUS_DETACHED)
		return flashStatus; // Only attach when detached
	EVE_CoCmd_d(phost, CMD_FLASHATTACH);
	EVE_Cmd_waitFlush(phost);					  // Wait for command completion
	return EVE_Hal_rd32(phost, REG_FLASH_STATUS); // Return current status
}

uint32_t EVE_CoCmd_flashFast(EVE_HalContext *phost, uint32_t *result)
{
	uint16_t resAddr;
	uint32_t flashStatus;

	if (!EVE_Cmd_waitFlush(phost))
	{
		if (result)
			*result = 0xE000;
		return EVE_Hal_rd32(phost, REG_FLASH_STATUS);
	} // Coprocessor must be ready
	flashStatus = EVE_Hal_rd32(phost, REG_FLASH_STATUS);
	if (flashStatus < FLASH_STATUS_BASIC)
	{
		if (result)
			*result = 0xE001;
		return flashStatus;
	}
	if (flashStatus > FLASH_STATUS_BASIC)
	{
		if (result)
			*result = 0;
		return flashStatus;
	} // Only enter fast mode when attached

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_FLASHFAST, 0))
		return flashStatus;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_FLASHFAST);
	resAddr = EVE_Cmd_moveWp(phost, 4); // Get the address where the coprocessor will write the result
	EVE_Cmd_endFunc(phost);
	EVE_Cmd_waitFlush(phost); // Wait for command completion
	if (result)
		*result = EVE_Hal_rd32(phost, RAM_CMD + resAddr); // Fetch result
	return EVE_Hal_rd32(phost, REG_FLASH_STATUS);		  // Return current status
}

void EVE_CoCmd_flashSpiDesel(EVE_HalContext *phost)
{
	EVE_CoCmd_d(phost, CMD_FLASHSPIDESEL);
}

void EVE_CoCmd_flashSpiTx(EVE_HalContext *phost, uint32_t num)
{
	EVE_CoCmd_dd(phost, CMD_FLASHSPITX, num);
}

void EVE_CoCmd_flashSpiRx(EVE_HalContext *phost, uint32_t ptr, uint32_t num)
{
	EVE_CoCmd_ddd(phost, CMD_FLASHSPIRX, ptr, num);
}

void EVE_CoCmd_flashSource(EVE_HalContext *phost, uint32_t ptr)
{
	EVE_CoCmd_dd(phost, CMD_FLASHSOURCE, ptr);
}

void EVE_CoCmd_appendF(EVE_HalContext *phost, uint32_t ptr, uint32_t num)
{
	EVE_CoCmd_ddd(phost, CMD_APPENDF, ptr, num);
}

bool EVE_CoCmd_loadImage_flash(EVE_HalContext *phost, uint32_t dst, uint32_t src, uint32_t *format)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_FLASHSOURCE, src))
		return false;
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_LOADIMAGE, dst))
		return false;
#endif

	if (!EVE_Cmd_waitFlush(phost))
		return false; // Coprocessor must be ready
	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_FLASHSOURCE);
	EVE_Cmd_wr32(phost, src);
	EVE_Cmd_wr32(phost, CMD_LOADIMAGE);
	EVE_Cmd_wr32(phost, dst);
	EVE_Cmd_wr32(phost, OPT_FLASH | OPT_NODL);
	EVE_Cmd_endFunc(phost);
	if (!EVE_Cmd_waitFlush(phost))
		return false; // Image failed to load
	if (format)
		*format = EVE_Hal_rd32(phost, 0x3097e8);
	return true;
}

bool EVE_CoCmd_inflate_flash(EVE_HalContext *phost, uint32_t dst, uint32_t src)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_FLASHSOURCE, src))
		return false;
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_INFLATE2, dst))
		return false;
#endif

	if (!EVE_Cmd_waitFlush(phost))
		return false; // Coprocessor must be ready
	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_FLASHSOURCE);
	EVE_Cmd_wr32(phost, src);
	EVE_Cmd_wr32(phost, CMD_INFLATE);
	EVE_Cmd_wr32(phost, dst);
	EVE_Cmd_wr32(phost, OPT_FLASH);
	EVE_Cmd_endFunc(phost);
	return EVE_Cmd_waitFlush(phost); // Image failed to load
}

void EVE_CoCmd_flashProgram(EVE_HalContext *phost, uint32_t dst, uint32_t src, uint32_t num)
{
	EVE_CoCmd_dddd(phost, CMD_FLASHPROGRAM, dst, src, num);
}

void EVE_CoCmd_animFrameRam(EVE_HalContext *phost, int16_t x, int16_t y, uint32_t aoptr, uint32_t frame)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_ANIMFRAMERAM, 0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_ANIMFRAMERAM);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_wr32(phost, aoptr);
	EVE_Cmd_wr32(phost, frame);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_animStartRam(EVE_HalContext *phost, int32_t ch, uint32_t aoptr, uint32_t loop)
{
	EVE_CoCmd_dddd(phost, CMD_ANIMSTARTRAM, ch, aoptr, loop);
}

void EVE_CoCmd_animStop(EVE_HalContext *phost, int32_t ch)
{
	EVE_CoCmd_dd(phost, CMD_ANIMSTOP, ch);
}

void EVE_CoCmd_animXY(EVE_HalContext *phost, int32_t ch, int16_t x, int16_t y)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_ANIMXY, d0))
		return;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_ANIMXY);
	EVE_Cmd_wr32(phost, ch);
	EVE_Cmd_wr16(phost, x);
	EVE_Cmd_wr16(phost, y);
	EVE_Cmd_endFunc(phost);
}

void EVE_CoCmd_animDraw(EVE_HalContext *phost, int32_t ch)
{
	EVE_CoCmd_dd(phost, CMD_ANIMDRAW, ch);
}

void EVE_CoCmd_runAnim(EVE_HalContext *phost, uint32_t waitmask, uint32_t play)
{
	EVE_CoCmd_ddd(phost, CMD_RUNANIM, waitmask, play);
}

bool EVE_CoCmd_memCrc(EVE_HalContext *phost, uint32_t ptr, uint32_t num, uint32_t *result)
{
	EVE_CoCmd_dddd(phost, CMD_MEMCRC, ptr, num, 0);
	READ_4_BYTE_RESULT(result, phost);
}

uint32_t EVE_CoCmd_regRead32(EVE_HalContext *phost, uint32_t ptr)
{
	uint32_t result = 0;
	EVE_CoCmd_regRead(phost, ptr, &result);
	return result;
}

bool EVE_CoCmd_regRead(EVE_HalContext *phost, uint32_t ptr, uint32_t *result)
{
	EVE_CoCmd_ddd(phost, CMD_REGREAD, ptr, 0);
	READ_4_BYTE_RESULT(result, phost);
}

void EVE_CoCmd_regWrite(EVE_HalContext *phost, uint32_t dst, uint32_t value)
{
	EVE_CoCmd_ddd(phost, CMD_REGWRITE, dst, value);
}

bool EVE_CoCmd_inflate_progMem(EVE_HalContext *phost, uint32_t dst, eve_progmem_const uint8_t *src, uint32_t size)
{
	if (!EVE_Cmd_waitFlush(phost))
		return false; // Coprocessor must be ready
	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_INFLATE);
	EVE_Cmd_wr32(phost, dst);
	EVE_Cmd_wr32(phost, 0); // options
	EVE_Cmd_wrProgMem(phost, src, (size + 3) & ~0x3UL);
	EVE_Cmd_endFunc(phost);
	return EVE_Cmd_waitFlush(phost); // Resource failed to load
}

/** @brief Get the end memory address of data inflated by CMD_INFLATE and CMD_INFLATE2
 * At API level 2, the allocation pointer is also advanced by:
 *  - cmd_loadimage
 *  - cmd_playvideo
 *  - cmd_videoframe
 *  - cmd_endlist
 */
bool EVE_CoCmd_getPtr(EVE_HalContext *phost, uint32_t *result)
{
	uint16_t resAddr;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_GETPTR, 0))
		return false;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_GETPTR);
	resAddr = EVE_Cmd_moveWp(phost, 4); // move write pointer to result location
	EVE_Cmd_endFunc(phost);

	/* Read result */
	if (result)
	{
		if (!EVE_Cmd_waitFlush(phost))
			return false;
		*result = EVE_Hal_rd32(phost, RAM_CMD + resAddr);
	}
	return true;
}

bool EVE_CoCmd_loadImage_progMem(EVE_HalContext *phost, uint32_t dst, eve_progmem_const uint8_t *src, uint32_t size, uint32_t *format)
{
#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_LOADIMAGE, dst))
		return false;
#endif

	if (!EVE_Cmd_waitFlush(phost))
		return false; // Coprocessor must be ready
	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_LOADIMAGE);
	EVE_Cmd_wr32(phost, dst);
	EVE_Cmd_wr32(phost, OPT_NODL);
	EVE_Cmd_wrProgMem(phost, src, (size + 3) & ~0x3UL);
	EVE_Cmd_endFunc(phost);
	if (!EVE_Cmd_waitFlush(phost))
		return false; // Image failed to load
	if (format)
		*format = EVE_Hal_rd32(phost, 0x3097e8);
	return true;
}

bool EVE_CoCmd_getProps(EVE_HalContext *phost, uint32_t *ptr, uint32_t *w, uint32_t *h)
{
	uint16_t resAddr;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_GETPROPS, 0))
		return false;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_GETPROPS);
	resAddr = EVE_Cmd_moveWp(phost, 12);
	EVE_Cmd_endFunc(phost);

	/* Read result */
	if (ptr || w || h)
	{
		if (!EVE_Cmd_waitFlush(phost))
			return false;
		if (ptr)
			*ptr = EVE_Hal_rd32(phost, RAM_CMD + resAddr);
		if (w)
			*w = EVE_Hal_rd32(phost, RAM_CMD + resAddr + 4);
		if (h)
			*h = EVE_Hal_rd32(phost, RAM_CMD + resAddr + 8);
	}
	return true;
}

bool EVE_CoCmd_getImage(EVE_HalContext *phost, uint32_t *source, uint32_t *fmt, uint32_t *w, uint32_t *h, uint32_t *palette)
{
	uint16_t resAddr;

#if EVE_CMD_HOOKS
	if (phost->CoCmdHook && phost->CoCmdHook(phost, CMD_GETIMAGE, 0))
		return false;
#endif

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_GETIMAGE);
	resAddr = EVE_Cmd_moveWp(phost, 4 * 5);
	EVE_Cmd_endFunc(phost);

	/* Read result */
	if (source || fmt || w || h || palette)
	{
		if (!EVE_Cmd_waitFlush(phost))
			return false;
		if (source)
			*source = EVE_Hal_rd32(phost, RAM_CMD + resAddr);
		if (fmt)
			*fmt = EVE_Hal_rd32(phost, RAM_CMD + resAddr + 4);
		if (w)
			*w = EVE_Hal_rd32(phost, RAM_CMD + resAddr + 8);
		if (h)
			*h = EVE_Hal_rd32(phost, RAM_CMD + resAddr + 12);
		if (palette)
			*palette = EVE_Hal_rd32(phost, RAM_CMD + resAddr + 16);
	}
	return true;
}

uint32_t EVE_CoCmd_fssize(EVE_HalContext *phost, const char *file, uint32_t *result)
{
	EVE_CoCmd_dsd(phost, CMD_FSSIZE, file, 0);
	READ_4_BYTE_RESULT(result, phost);
}

uint32_t EVE_CoCmd_fsread(EVE_HalContext *phost, uint32_t dst, const char *file, uint32_t *result)
{
	EVE_CoCmd_ddsd(phost, CMD_FSREAD, dst, file, 0);
	READ_4_BYTE_RESULT(result, phost);
}
uint32_t EVE_CoCmd_fsfile(EVE_HalContext *phost, uint32_t size, const char *file, uint32_t *result)
{
	EVE_CoCmd_ddsd(phost, CMD_FSFILE, size, file, 0);
	READ_4_BYTE_RESULT(result, phost);
}
uint32_t EVE_CoCmd_fswrite(EVE_HalContext *phost, uint32_t dst, const char *file, uint32_t *result)
{
	EVE_CoCmd_ddsd(phost, CMD_FSWRITE, dst, file, 0);
	READ_4_BYTE_RESULT(result, phost);
}

uint32_t EVE_CoCmd_fssource(EVE_HalContext *phost, const char *file, uint32_t *result)
{
	EVE_CoCmd_dsd(phost, CMD_FSSOURCE, file, 0);

	READ_4_BYTE_RESULT(result, phost);
}

void EVE_CoCmd_loadPatch(EVE_HalContext *phost, uint32_t options)
{
	EVE_CoCmd_dd(phost, CMD_LOADPATCH, options);
}
void EVE_CoCmd_messageBox(EVE_HalContext *phost, uint32_t font, uint32_t options, const char *msg)
{
	EVE_CoCmd_ddds(phost, CMD_MESSAGEBOX, font, options, msg);
}

/**
 * @brief Send CMD_SDATTACH.
 *
 * @param phost Pointer to Hal context
 * @param options Command option
 * @param result
 */
uint32_t EVE_CoCmd_sdattach(EVE_HalContext *phost, uint32_t options, uint32_t *result)
{
	EVE_CoCmd_ddd(phost, CMD_SDATTACH, options, 0);
	READ_4_BYTE_RESULT(result, phost);
}
/* end of file */
