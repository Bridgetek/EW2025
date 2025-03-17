/**
 * @file EVE_CoCmd.h
 * @brief EVE's co-processor commands
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

#ifndef EVE_CO_CMD__H
#define EVE_CO_CMD__H

/*********************
 *      INCLUDES
 *********************/
#include "EVE_HalDefs.h"
#include "EVE_GpuDefs.h"

/**********************
 *      MACROS
 **********************/
/** Change to `eve_pragma_error` to strictly disable use of unsupported commands */
#define EVE_COCMD_UNSUPPORTED(cmd, res) eve_pragma_warning("Coprocessor command " #cmd " is not supported on this platform")(res)

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/** @name Reusable templates for basic commands to save on compiled code space
 * d: uint32_t, w: uint16_t
 */
///@{
void EVE_CoCmd_d(EVE_HalContext *phost, uint32_t cmd);
void EVE_CoCmd_dd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0);
void EVE_CoCmd_ddd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, uint32_t d1);
void EVE_CoCmd_ddsd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, const char *s, uint32_t d1);
void EVE_CoCmd_dds(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, const char *s);
void EVE_CoCmd_ddds(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, uint32_t d1, const char *s);
void EVE_CoCmd_dsd(EVE_HalContext *phost, uint32_t cmd, const char *s, uint32_t d0);
void EVE_CoCmd_dddd(EVE_HalContext *phost, uint32_t cmd, uint32_t d0, uint32_t d1, uint32_t d2);
void EVE_CoCmd_ddwww(EVE_HalContext *phost, uint32_t cmd,
                     uint32_t d0, uint16_t w1, uint16_t w2, uint16_t w3);
void EVE_CoCmd_dwwdwwd(EVE_HalContext *phost, uint32_t cmd,
                       uint16_t w0, uint16_t w1, uint32_t d2,
                       uint16_t w3, uint16_t w4, uint32_t d5);
void EVE_CoCmd_dwwww(EVE_HalContext *phost, uint32_t cmd,
                     uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3);
void EVE_CoCmd_dwwwww(EVE_HalContext *phost, uint32_t cmd,
                      uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4);
void EVE_CoCmd_dwwwwww(EVE_HalContext *phost, uint32_t cmd,
                       uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4, uint16_t w5);
void EVE_CoCmd_dwwwwwww(EVE_HalContext *phost, uint32_t cmd,
                        uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3,
                        uint16_t w4, uint16_t w5, uint16_t w6);
void EVE_CoCmd_dwwwwwwww(EVE_HalContext *phost, uint32_t cmd,
                         uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3,
                         uint16_t w4, uint16_t w5, uint16_t w6, uint16_t w7);
///@}

/*
Commands are organized as follows:
 - System commands
 - I/O commands
 - Render state commands
 - Widget rendering commands
 */

/**********************************************************************
*************************** EVE_CoCmd.c *******************************
**********************************************************************/
/**
 * @brief Send CMD_DLSTART
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_dlStart(EVE_HalContext *phost);

/**
 * @brief Send CMD_SWAP
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_swap(EVE_HalContext *phost);

/**
 * @brief Send CMD_INTERRUPT
 *
 * @param phost Pointer to Hal context
 * @param ms Delay before interrupt triggers, in milliseconds. The interrupt is guaranteed not to fire before this delay. If ms is zero, the interrupt fires immediately
 */
void EVE_CoCmd_interrupt(EVE_HalContext *phost, uint32_t ms);

/**
 * @brief Send CMD_COLDSTART
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_coldStart(EVE_HalContext *phost);

/**
 * @brief Send CMD_SYNC
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_sync(EVE_HalContext *phost);

/**
 * @brief Send CMD_CLEARCACHE
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_clearCache(EVE_HalContext *phost);
/**
 * @brief Send CMD_NOP
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_nop(EVE_HalContext *phost);

/**
 * @brief Send CMD_WAIT
 *
 * @param phost Pointer to Hal context
 * @param us microsecond
 */
void EVE_CoCmd_wait(EVE_HalContext *phost, uint32_t us);

/**
 * @brief Send CMD_RETURN
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_return(EVE_HalContext *phost);
/**
 * @brief Send CMD_CALLLIST
 *
 * @param phost Pointer to Hal context
 * @param a memory address of the command list
 */
void EVE_CoCmd_callList(EVE_HalContext *phost, uint32_t a);

/**
 * @brief Send CMD_NEWLIST
 *
 * @param phost Pointer to Hal context
 * @param a memory address of start of command list
 */
void EVE_CoCmd_newList(EVE_HalContext *phost, uint32_t a);

/**
 * @brief Send CMD_ENDLIST
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_endList(EVE_HalContext *phost);
/**
 * @brief Send CMD_GETMATRIX
 *
 * @param phost Pointer to Hal context
 * @param m output parameters; 6 values; written with matrix coeffcients a, b, c, d, e, f
 */
bool EVE_CoCmd_getMatrix(EVE_HalContext *phost, int32_t *m);

/**
 * @brief CMD_SETROTATE
 *
 * @param phost Pointer to Hal context
 * @param r new rotation value 0-7
 */
void EVE_CoCmd_setRotate(EVE_HalContext *phost, uint32_t r);

/**
 * @brief CMD_WATCHDOG
 *
 * @param phost Pointer to Hal context
 * @param init_val watchdog interval
 */
void EVE_CoCmd_watchdog(EVE_HalContext *phost, uint32_t init_val);

/**
 * @brief Send CMD_CALIBRATE
 *
 * @param phost Pointer to Hal context
 * @param result
 * @return uint32_t
 */
uint32_t EVE_CoCmd_calibrate(EVE_HalContext *phost, uint32_t result);

/**
 * @brief Send CMD_CALIBRATESUB
 *
 * @param phost Pointer to Hal context
 * @param x X screen
 * @param y Y screen
 * @param w Window width
 * @param h Window height
 * @param result
 * @return uint32_t output parameter; written with 0 on failure
 */
uint32_t EVE_CoCmd_calibrateSub(EVE_HalContext *phost, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t result);

/**
 * @brief Send CMD_SPINNER
 *
 * @param phost Pointer to Hal context
 * @param x The X coordinate of top left of spinner
 * @param y The Y coordinate of top left of spinner
 * @param style The style of spinner. Valid range is from 0 to 3
 * @param scale The scaling coefficient of spinner. 0 means no scaling
 */
void EVE_CoCmd_spinner(EVE_HalContext *phost, int16_t x, int16_t y, uint16_t style, uint16_t scale);

/**
 * @brief Send CMD_STOP
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_stop(EVE_HalContext *phost);

/**
 * @brief Send CMD_TRACK
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of track area top-left, in pixels
 * @param y y-coordinate of track area top-left, in pixels
 * @param w width of track area, in pixels
 * @param h height of track area, in pixels
 * @param tag tag for this track, 1-255
 */
void EVE_CoCmd_track(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, int16_t tag);

/**
 * @brief Send CMD_SCREENSAVER
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_screenSaver(EVE_HalContext *phost);

/**
 * @brief Send CMD_SKETCH
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of sketch area top-left, in pixels
 * @param y y-coordinate of sketch area top-left, in pixels
 * @param w Width of sketch area, in pixels
 * @param h Height of sketch area, in pixels
 * @param ptr Base address of sketch bitmap
 * @param format Format of sketch bitmap, either L1 or L8
 */
void EVE_CoCmd_sketch(EVE_HalContext *phost, int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t ptr, uint16_t format);

/**
 * @brief Send CMD_LOGO
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_logo(EVE_HalContext *phost);

/**
 * @brief Send CMD_TESTCARD
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_testCard(EVE_HalContext *phost);

/**
 * @brief Send CMD_RENDERTAEGET
 *
 * @param phost Pointer to Hal context
 * @param source Source address for bitmap, in RAM or ï¬‚ash, as a BITMAP SOURCEargument
 * @param fmt Bitmap format
 * @param w Bitmap screen width in pixels
 * @param h Bitmap screen height in pixels
 */
void EVE_CoCmd_renderTarget(EVE_HalContext *phost, uint32_t dst, uint16_t fmt, int16_t w, int16_t h);

/**********************************************************************
************************ EVE_CoCmd_IO.c *******************************
**********************************************************************/
/**
 * @brief Send CMD_MEMWRITE.
 *
 * You must write the specified number of bytes using one
 * or more of any EVE_Cmd_wr... function call after calling this command,
 * and pad the written data to align at 4 bytes
 *
 * @param phost Pointer to Hal context
 * @param ptr Destination on RAM_G
 * @param num number of bytes to copy
 */
void EVE_CoCmd_memWrite(EVE_HalContext *phost, uint32_t ptr, uint32_t num);

/**
 * @brief Send CMD_MEMWRITE, followed by a single 32-bit value.
 *
 * Convenience function to write one 32-bit value through the coprocessor.
 * Useful in combination with EVE_Cmd_waitRead32 to add synchronization points into the command buffer.
 *
 * @param phost Pointer to Hal context
 * @param ptr Destination on RAM_G
 * @param value Value to write
 */
void EVE_CoCmd_memWrite32(EVE_HalContext *phost, uint32_t ptr, uint32_t value);

/**
 * @brief Send CMD_MEMSET
 *
 * @param phost Pointer to Hal context
 * @param ptr starting address of the memory block
 * @param value value to be written to memory
 * @param num number of bytes in the memory block
 */
void EVE_CoCmd_memSet(EVE_HalContext *phost, uint32_t ptr, uint32_t value, uint32_t num);

/**
 * @brief Send CMD_MEMZERO
 *
 * @param phost Pointer to Hal context
 * @param ptr Destination on RAM_G
 * @param num number of bytes to clear
 */
void EVE_CoCmd_memZero(EVE_HalContext *phost, uint32_t ptr, uint32_t num);

/**
 * @brief Send CMD_MEMCPY
 *
 * @param phost Pointer to Hal context
 * @param dest address of the destination memory block
 * @param src address of the source memory block
 * @param num number of bytes to copy
 */
void EVE_CoCmd_memCpy(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num);

/**
 * @brief Send CMD_APPEND
 *
 * @param phost  Pointer to Hal context
 * @param ptr Start of source commands in main memory
 * @param num Number of bytes to copy. This must be a multiple of 4
 */
void EVE_CoCmd_append(EVE_HalContext *phost, uint32_t ptr, uint32_t num);

/**
 * @brief Send CMD_SNAPSHOT
 *
 * @param phost Pointer to Hal context
 * @param ptr Snapshot destination address, in RAM_G
 */
void EVE_CoCmd_snapshot(EVE_HalContext *phost, uint32_t ptr);

/**
 * @brief Send CMD_INFLATE. Data must follow this command
 *
 * @param phost Pointer to Hal context
 * @param ptr Destination address
 * @param options
 */
void EVE_CoCmd_inflate(EVE_HalContext *phost, uint32_t ptr, uint32_t options);

/**
 * @brief Send CMD_LOADIMAGE. Data must follow this command
 *
 * @param phost Pointer to Hal context
 * @param ptr Destination address
 * @param options Command option
 */
void EVE_CoCmd_loadImage(EVE_HalContext *phost, uint32_t ptr, uint32_t options);

/**
 * @brief Send CMD_MEDIAFIFO. See EVE_MediaFifo interface for easier FIFO access
 *
 * @param phost Pointer to Hal context
 * @param ptr starting address of the memory block, 4-byte aligned
 * @param size number of bytes in the source memory block, 4-byte aligned
 */
void EVE_CoCmd_mediaFifo(EVE_HalContext *phost, uint32_t ptr, uint32_t size);

/**
 * @brief Send CMD_VIDEOSTART
 *
 * @param phost
 * @param options
 */
void EVE_CoCmd_videoStart(EVE_HalContext *phost, uint32_t options);
/**
 * @brief CMD_VIDEOFRAME
 *
 * @param phost Pointer to Hal context
 * @param dst Main memory location to load the frame data
 * @param ptr Completion pointer
 */
void EVE_CoCmd_videoFrame(EVE_HalContext *phost, uint32_t dst, uint32_t ptr);

/**
 * @brief Send CMD_PLAYVIDEO
 *
 * @param phost Pointer to Hal context
 * @param options Command option
 */
void EVE_CoCmd_playVideo(EVE_HalContext *phost, uint32_t options);

/**
 * @brief Send CMD_FLASHERASE
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_flashErase(EVE_HalContext *phost);

/**
 * @brief Send CMD_FLASHERASE. Wait for completion
 *
 * @param phost Pointer to Hal context
 * @return bool false on coprocessor fault
 */
bool EVE_CoCmd_flashErase_flush(EVE_HalContext *phost);

/**
 * @brief Send CMD_FLASHWRITE. This command must be followed by the data to write
 *
 * @param phost Pointer to Hal context
 * @param ptr destination address in flash memory. Must be 256-byte aligned
 * @param num number of bytes to write, must be multiple of 256
 */
void EVE_CoCmd_flashWrite(EVE_HalContext *phost, uint32_t ptr, uint32_t num);

/**
 * @brief Read from Flash to RAM_G. Call EVE_Cmd_waitFlush to wait for completion
 *
 * @param phost Pointer to Hal context
 * @param dest destination address in main memory. Must be 4-byte aligned
 * @param src source address in flash memory. Must be 64-byte aligned
 * @param num number of bytes to read, must be multiple of 4
 */
void EVE_CoCmd_flashRead(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num);

/**
 * @brief Read from Flash to RAM_G
 *
 * @param phost Pointer to Hal context
 * @param dest destination address in main memory. Must be 4-byte aligned
 * @param src source address in flash memory. Must be 64-byte aligned
 * @param num number of bytes to read, must be multiple of 4
 * @return bool false on coprocessor fault
 */
bool EVE_CoCmd_flashRead_flush(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num);

/**
 * @brief Send CMD_FLASHUPDATE. This command must be followed by the data to write
 *
 * @param phost Pointer to Hal context
 * @param dest destination address in flash memory. Must be 4096-byte aligned
 * @param src source data in main memory. Must be 4-byte aligned
 * @param num number of bytes to write, must be multiple of 4096
 */
void EVE_CoCmd_flashUpdate(EVE_HalContext *phost, uint32_t dest, uint32_t src, uint32_t num);

/**
 * @brief Send CMD_FLASHDETACH
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_flashDetach(EVE_HalContext *phost);

/**
 * @brief Attach flash
 *
 * @param phost Pointer to Hal context
 * @return uint32_t Returns new FLASH_STATUS
 */
uint32_t EVE_CoCmd_flashAttach(EVE_HalContext *phost);

/**
Enter fast flash state. Returns new FLASH_STATUS. Optional parameter `result` will contain any error code, 0 on success
\n 0xE001 flash is not attached
\n 0xE002 no header detected in sector 0 - is flash blank?
\n 0xE003 sector 0 data failed integrity check
\n 0xE004 device/blob mismatch - was correct blob loaded?
\n 0xE005 failed full-speed test - check board wiring
*/
uint32_t EVE_CoCmd_flashFast(EVE_HalContext *phost, uint32_t *result);

/**
 * @brief Send CMD_FLASHSPIDESEL
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_flashSpiDesel(EVE_HalContext *phost);

/**
 * @brief Send CMD_FLASHSPITX. Must follow data
 *
 * @param phost Pointer to Hal context
 * @param num number of bytes to transmit
 */
void EVE_CoCmd_flashSpiTx(EVE_HalContext *phost, uint32_t num);

/**
 * @brief Send CMD_FLASHSPIRX
 *
 * @param phost Pointer to Hal context
 * @param ptr destination address in main memory
 * @param num number of bytes to receive
 */
void EVE_CoCmd_flashSpiRx(EVE_HalContext *phost, uint32_t ptr, uint32_t num);

/**
 * @brief Send CMD_FLASHSOURCE
 *
 * @param phost Pointer to Hal context
 * @param ptr flash address, must be 64-byte aligned
 */
void EVE_CoCmd_flashSource(EVE_HalContext *phost, uint32_t ptr);

/**
 * @brief Send CMD_APPENDF
 *
 * @param phost Pointer to Hal context
 * @param ptr Start of source commands in flash memory. Must be 64-byte aligned
 * @param num Number of bytes to copy. This must be a multiple of 4
 */
void EVE_CoCmd_appendF(EVE_HalContext *phost, uint32_t ptr, uint32_t num);

/**
 * @brief Load image from Flash to RAM_G
 *
 * @param phost Pointer to Hal context
 * @param dst Image location on flash
 * @param src Destination on RAM_G
 * @param format Output parameter format returns loaded bitmap format on success
 * @return bool Returns false on coprocessor fault
 */
bool EVE_CoCmd_loadImage_flash(EVE_HalContext *phost, uint32_t dst, uint32_t src, uint32_t *format);

/**
 * @brief Inflates data from Flash to RAM_G
 *
 * @param phost Pointer to Hal context
 * @param dst Image location on flash
 * @param src Destination on RAM_G
 * @return bool Returns false on coprocessor fault
 */
bool EVE_CoCmd_inflate_flash(EVE_HalContext *phost, uint32_t dst, uint32_t src);

/**
 * @brief Send CMD_FLASHPROGRAM
 *
 * @param phost Pointer to Hal context
 * @param dst destination address in flash memory. Must be 4096-byte aligned
 * @param src source data in main memory. Must be 4-byte aligned
 * @param num number of bytes to write, must be multiple of 4096
 */
void EVE_CoCmd_flashProgram(EVE_HalContext *phost, uint32_t dst, uint32_t src, uint32_t num);

/**
 * @brief Send CMD_ANIMFRAMERAM
 *
 * @param phost  Pointer to Hal context
 * @param x X screen
 * @param y Y screen
 * @param aoptr The address of the animation object in flash memory
 * @param frame Frame Frame number to draw, starting from zero
 */
void EVE_CoCmd_animFrameRam(EVE_HalContext *phost, int16_t x, int16_t y, uint32_t aoptr, uint32_t frame);

/**
 * @brief Send CMD_ANIMSTARTRAM
 *
 * @param phost  Pointer to Hal context
 * @param ch Channel
 * @param aoptr The address of the animation object in flash memory.
 * @param loop Loop flags
 */
void EVE_CoCmd_animStartRam(EVE_HalContext *phost, int32_t ch, uint32_t aoptr, uint32_t loop);

/**
 * @brief Send CMD_ANIMSTOP
 *
 * @param phost  Pointer to Hal context
 * @param ch Channel
 */
void EVE_CoCmd_animStop(EVE_HalContext *phost, int32_t ch);

/**
 * @brief Send CMD_ANIMXY
 *
 * @param phost  Pointer to Hal context
 * @param ch Channel
 * @param x X screen
 * @param y Y screen
 */
void EVE_CoCmd_animXY(EVE_HalContext *phost, int32_t ch, int16_t x, int16_t y);

/**
 * @brief Send CMD_ANIMDRAW
 *
 * @param phost  Pointer to Hal context
 * @param ch Channel
 */
void EVE_CoCmd_animDraw(EVE_HalContext *phost, int32_t ch);

/**
 * @brief Send CMD_RUNANIM
 *
 * @param phost Pointer to Hal context
 * @param waitmask Wait mask
 * @param play Play
 */
void EVE_CoCmd_runAnim(EVE_HalContext *phost, uint32_t waitmask, uint32_t play);

/**
 * @brief Send CMD_MEMCRC
 *
 * @param phost Pointer to Hal context
 * @param ptr starting address of the memory block
 * @param num number of bytes in the source memory block
 * @param result output parameter; written with the CRC-32 after command execution
 * @return bool Returns false in case of error
 */
bool EVE_CoCmd_memCrc(EVE_HalContext *phost, uint32_t ptr, uint32_t num, uint32_t *result);

/**
 * @brief Send CMD_REGREAD
 *
 * @param phost Pointer to Hal context
 * @param ptr address of register to read
 * @param result output parameter; written with the register value
 * @return bool Returns false in case of error
 */
bool EVE_CoCmd_regRead(EVE_HalContext *phost, uint32_t ptr, uint32_t *result);
uint32_t EVE_CoCmd_regRead32(EVE_HalContext *phost, uint32_t ptr);
void EVE_CoCmd_regWrite(EVE_HalContext *phost, uint32_t dst, uint32_t value);

/**
 * @brief Inflates data from program memory to RAM_G
 *
 * @param phost Pointer to Hal context
 * @param dst Image address
 * @param src Desination on RAM_G
 * @param size size of `src` in bytes
 * @return Returns false on coprocessor fault
 */
bool EVE_CoCmd_inflate_progMem(EVE_HalContext *phost, uint32_t dst, eve_progmem_const uint8_t *src, uint32_t size);

/**
 * @brief Get the end memory address of data inflated by CMD_INFLATE
 *
 * @param phost Pointer to Hal context
 * @param result memory address
 */
bool EVE_CoCmd_getPtr(EVE_HalContext *phost, uint32_t *result);

/**
 * @brief Load image from program memory
 *
 * @param phost Pointer to Hal context
 * @param dst Image address
 * @param src Desination on RAM_G
 * @param size size of `src` in bytes
 * @param format Output parameter format returns loaded bitmap format on success
 * @return bool Returns false on coprocessor fault
 */
bool EVE_CoCmd_loadImage_progMem(EVE_HalContext *phost, uint32_t dst, eve_progmem_const uint8_t *src, uint32_t size, uint32_t *format);

/**
 * @brief Get the image properties decompressed by CMD_LOADIMAGE
 *
 * @param phost Pointer to Hal context
 * @param ptr Source address of bitmap
 * @param w Width of bitmap, in pixels
 * @param h Height of bitmap, in pixels
 * @return bool Returns false on coprocessor fault
 */
bool EVE_CoCmd_getProps(EVE_HalContext *phost, uint32_t *ptr, uint32_t *w, uint32_t *h);

/**
 * @brief Send CMD_GETIMAGE
 *
 * @param phost Pointer to Hal context
 * @param source Source address of bitmap
 * @param fmt Format of the bitmap
 * @param w Width of bitmap, in pixels
 * @param h Height of bitmap, in pixels
 * @param palette palette data of the bitmap if fmt is PALETTED565 or PALETTED4444. Otherwise zero
 * @return bool False on coprocessor error
 */
bool EVE_CoCmd_getImage(EVE_HalContext *phost, uint32_t *source, uint32_t *fmt, uint32_t *w, uint32_t *h, uint32_t *palette);

/**
 * @brief Send CMD_SDATTACH.
 *
 * @param phost Pointer to Hal context
 * @param options Command option
 * @param result
 */
uint32_t EVE_CoCmd_sdattach(EVE_HalContext *phost, uint32_t options, uint32_t *result);

uint32_t EVE_CoCmd_fssize(EVE_HalContext *phost, const char *file, uint32_t *result);
uint32_t EVE_CoCmd_fsfile(EVE_HalContext *phost, uint32_t size, const char *file, uint32_t *result);
uint32_t EVE_CoCmd_fswrite(EVE_HalContext *phost, uint32_t dst, const char *file, uint32_t *result);
uint32_t EVE_CoCmd_fsread(EVE_HalContext *phost, uint32_t dst, const char *file, uint32_t *result);

/**
 * @brief Send CMD_FSSOURCE.
 *
 * @param phost Pointer to Hal context
 * @param file file name
 * @param result
 */

uint32_t EVE_CoCmd_fssource(EVE_HalContext *phost, const char *file, uint32_t *result);
void EVE_CoCmd_loadPatch(EVE_HalContext *phost, uint32_t options);
void EVE_CoCmd_messageBox(EVE_HalContext *phost, uint32_t font, uint32_t options, const char *msg);

/**********************************************************************
************************* EVE_CoCmd_State.c ***************************
**********************************************************************/
/**
 * @brief Send CMD_BGCOLOR
 *
 * @param phost Pointer to Hal context
 * @param c New background color
 */
void EVE_CoCmd_bgColor(EVE_HalContext *phost, uint32_t c);

/**
 * @brief Send CMD_FGCOLOR
 *
 * @param phost Pointer to Hal context
 * @param c New foreground color, as a 24-bit RGB number
 */
void EVE_CoCmd_fence(EVE_HalContext *phost);
void EVE_CoCmd_fgColor(EVE_HalContext *phost, uint32_t c);

void EVE_CoCmd_loadasset(EVE_HalContext *phost, uint32_t ptr, uint32_t options);

/**
 * @brief Send CMD_LOADIDENTITY
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_loadIdentity(EVE_HalContext *phost);

/**
 * @brief Send CMD_TRANSLATE
 *
 * @param phost Pointer to Hal context
 * @param tx x scale factor, in signed 16.16 bit fixed-point form.
 * @param ty y scale factor, in signed 16.16 bit fixed-point form.
 */
void EVE_CoCmd_translate(EVE_HalContext *phost, int32_t tx, int32_t ty);

/**
 * @brief Send CMD_SCALE
 *
 * @param phost Pointer to Hal context
 * @param sx x scale factor, in signed 16.16 bit fixed-point form
 * @param sy y scale factor, in signed 16.16 bit fixed-point form
 */
void EVE_CoCmd_scale(EVE_HalContext *phost, int32_t sx, int32_t sy);

/**
 * @brief Send CMD_ROTATE
 *
 * @param phost Pointer to Hal context
 * @param a clockwise rotation angle, in units of 1/65536 of a circle
 */
void EVE_CoCmd_rotate(EVE_HalContext *phost, int32_t a);

/**
 * @brief Send CMD_SETMATRIX
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_setMatrix(EVE_HalContext *phost);

/**
 * @brief Send CMD_SETFONT
 *
 * @param phost Pointer to Hal context
 * @param font bitmap handle number, 0-31
 * @param ptr pointer to font description block
 * @param firstchar
 */
void EVE_CoCmd_setFont(EVE_HalContext *phost, uint32_t font, uint32_t ptr, uint32_t firstchar);
/**
 * @brief Send CMD_GRADCOLOR
 *
 * @param phost Pointer to Hal context
 * @param c New highlight gradient color, as a 24-bit RGB number
 */
void EVE_CoCmd_gradColor(EVE_HalContext *phost, uint32_t c);

/**
 * @brief Send CMD_SETBASE
 *
 * @param phost Pointer to Hal context
 * @param base Numeric base, from 2 to 36
 */
void EVE_CoCmd_setBase(EVE_HalContext *phost, uint32_t base);

/**
 * @brief Send CMD_SETSCRATCH
 *
 * @param phost Pointer to Hal context
 * @param handle bitmap handle number, 0-31
 */
void EVE_CoCmd_setScratch(EVE_HalContext *phost, uint32_t handle);

/**
 * @brief Send CMD_ROMFONT
 *
 * @param phost Pointer to Hal context
 * @param font Font number
 * @param romslot Slot number
 */
void EVE_CoCmd_romFont(EVE_HalContext *phost, uint32_t font, uint32_t romslot);

/**
 * @brief Send CMD_SETBITMAP
 *
 * @param phost Pointer to Hal context
 * @param source Source address for bitmap, in RAM or ï¬‚ash, as a BITMAP SOURCEargument
 * @param fmt Bitmap format
 * @param w Bitmap screen width in pixels
 * @param h Bitmap screen height in pixels
 */
void EVE_CoCmd_setBitmap(EVE_HalContext *phost, uint32_t source, uint16_t fmt, uint16_t w, uint16_t h);

/**
 * @brief Send CMD_ROTATEAROUND
 *
 * @param phost Pointer to Hal context
 * @param x center of rotation/scaling, x-coordinate
 * @param y center of rotation/scaling, y-coordinate
 * @param a clockwise rotation angle, in units of 1/65536 of a circle
 * @param s scale factor, in signed 16.16 bit fixed-point form
 */
void EVE_CoCmd_rotateAround(EVE_HalContext *phost, int32_t x, int32_t y, int32_t a, int32_t s);

/**
 * @brief Send CMD_RESETFONT
 *
 * @param phost Pointer to Hal context
 */
void EVE_CoCmd_resetFonts(EVE_HalContext *phost);

/**
 * @brief Send CMD_FILLWIDTH
 *
 * @param phost Pointer to Hal context
 * @param s line fill width, in pixels
 */
void EVE_CoCmd_fillWidth(EVE_HalContext *phost, uint32_t s);

/**
 * @brief Send CMD_BITMAP_TRANSFORM. Blocking call if a pointer is passed to `result`
 *
 * @param phost Pointer to Hal context
 * @param x0 Point X0 screen coordinate, in pixels
 * @param y0 Point Y0 screen coordinate, in pixels
 * @param x1 Point X1 screen coordinate, in pixels
 * @param y1 Point Y1 screen coordinate, in pixels
 * @param x2 Point X2 screen coordinate, in pixels
 * @param y2 Point Y2 screen coordinate, in pixels
 * @param tx0 Point X0 bitmap coordinate, in pixels
 * @param ty0 Point Y0 bitmap coordinate, in pixels
 * @param tx1 Point X1 bitmap coordinate, in pixels
 * @param ty1 Point Y1 bitmap coordinate, in pixels
 * @param tx2 Point X2 bitmap coordinate, in pixels
 * @param ty2 Point Y2 bitmap coordinate, in pixels
 * @param result Optional parameter `result` is set to -1 on success, 0 on failure
 * @return bool Returns false on coprocessor error
 */
bool EVE_CoCmd_bitmapTransform(EVE_HalContext *phost, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t tx0, int32_t ty0, int32_t tx1, int32_t ty1, int32_t tx2, int32_t ty2, uint16_t *result);

/**********************************************************************
*********************** EVE_CoCmd_Widgets.c ***************************
**********************************************************************/
/**
 * @brief Send CMD_TEXT
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of text base, in pixels
 * @param y y-coordinate of text base, in pixels
 * @param font Font to use for text, 0-31
 * @param options Text option
 * @param s Text string, UTF-8 encoding
 * @param ... Text format
 */
void EVE_CoCmd_text(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, const char *s, ...);

/**
 * @brief Send CMD_TEXT with length
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of text base, in pixels
 * @param y y-coordinate of text base, in pixels
 * @param font Font to use for text, 0-31
 * @param options Text option
 * @param s Text string, UTF-8 encoding
 * @param length length of text
 */
void EVE_CoCmd_text_s(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, const char *s, uint32_t length);

/**
 * @brief Send CMD_TEXT
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of text base, in pixels
 * @param y y-coordinate of text base, in pixels
 * @param font Font to use for text, 0-31
 * @param options Text option
 * @param bottom
 * @param baseLine
 * @param capsHeight
 * @param xOffset
 * @param s Text string, UTF-8 encoding
 */
void EVE_CoCmd_text_ex(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, bool bottom, int16_t baseLine, int16_t capsHeight, int16_t xOffset, const char *s);

/**
 * @brief Send CMD_BUTTON
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of button top-left, in pixels
 * @param y y-coordinate of button top-left, in pixels
 * @param w Button width
 * @param h Button height
 * @param font font to use for text, 0-31
 * @param options Font option
 * @param s Button label text, UTF-8 encoding
 * @param ... Format of button label text, like printf
 */
void EVE_CoCmd_button(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const char *s, ...);

/**
 * @brief Send CMD_KEYS
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of keys top-left, in pixels
 * @param y y-coordinate of keys top-left, in pixels
 * @param w Keys width
 * @param h Keys height
 * @param font Font for keys label
 * @param options Drawing option
 * @param s key labels, one character per key
 */
void EVE_CoCmd_keys(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const char *s);

/**
 * @brief Send CMD_TOGGLE
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of top-left of toggle, in pixels
 * @param y y-coordinate of top-left of toggle, in pixels
 * @param w width of toggle, in pixels
 * @param font font to use for text, 0-31
 * @param options Drawing option
 * @param state state of the toggle: 0 is off, 65535 is on.
 * @param s string labels for toggle,UTF-8 encoding
 * @param ... string labels format
 */
void EVE_CoCmd_toggle(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t font, uint16_t options, uint16_t state, const char *s, ...);

/**
 * @brief Send CMD_NUMBER
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of text base, in pixels
 * @param y y-coordinate of text base, in pixels
 * @param font font to use for text, 0-31
 * @param options Drawing option
 * @param n The number to display, either unsigned or signed 32-bit
 */
void EVE_CoCmd_number(EVE_HalContext *phost, int16_t x, int16_t y, int16_t font, uint16_t options, int32_t n);

/**
 * @brief Send CMD_PROGRESS
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of progress bar top-left, in pixels
 * @param y y-coordinate of progress bar top-left, in pixels
 * @param w width of progress bar, in pixels
 * @param h height of progress bar, in pixels
 * @param options Drawing option
 * @param val Displayed value of progress bar, between 0 and range inclusive
 * @param range Maximum value
 */
void EVE_CoCmd_progress(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);

/**
 * @brief Send CMD_SLIDER
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of slider top-left, in pixels
 * @param y y-coordinate of slider top-left, in pixels
 * @param w width of slider, in pixels
 * @param h height of slider, in pixels
 * @param options By default the slider is drawn with a 3D effect. OPT_FLAT removes the 3D effect
 * @param val Displayed value of slider, between 0 and range inclusive
 * @param range Maximum value
 */
void EVE_CoCmd_slider(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);

/**
 * @brief Send CMD_SCROLLBAR
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of scroll bar top-left, in pixels
 * @param y y-coordinate of scroll bar top-left, in pixels
 * @param w width of scroll bar, in pixels. If width is greater, the scroll bar is drawn horizontally
 * @param h height of scroll bar, in pixels. If height is greater, the scroll bar is drawn vertically
 * @param options By default the scroll bar is drawn with a 3D eï¬€ect. OPT FLAT removes the 3D eï¬€ect
 * @param val Displayed value of scroll bar, between 0 and range inclusive
 * @param size Scroll bar size
 * @param range Maximum value
 */
void EVE_CoCmd_scrollbar(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t size, uint16_t range);

/**
 * @brief Send CMD_GAUGE
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of gauge center, in pixels
 * @param y y-coordinate of gauge center, in pixels
 * @param r radius of the gauge, in pixels
 * @param options Drawing option
 * @param major Number of major subdivisions on the dial, 1-10
 * @param minor Number of minor subdivisions on the dial, 1-10
 * @param val gauge indicated value, between 0 and range, inclusive
 * @param range maximum value
 */
void EVE_CoCmd_gauge(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range);

/**
 * @brief Send CMD_CLOCK
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of clock center, in pixels
 * @param y y-coordinate of clock center, in pixels
 * @param r Clock radius
 * @param options Drawing option
 * @param h hours
 * @param m minutes
 * @param s seconds
 * @param ms milliseconds
 */
void EVE_CoCmd_clock(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms);

/**
 * @brief Send CMD_DIAL
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of dial center, in pixels
 * @param y y-coordinate of dial center, in pixels
 * @param r radius of dial, in pixels.
 * @param options Drawing option
 * @param val Specify the position of dial points by setting value between 0 and 65535 inclusive. 0 means that the dial points straight down, 0x4000 left, 0x8000 up, and0xc000 right.
 */
void EVE_CoCmd_dial(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t val);

/**
 * @brief Send CMD_ARC
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of dial center, in pixels
 * @param y y-coordinate of dial center, in pixels
 * @param r0 inner radius of arc, in pixels.
 * @param r1 outer radius of arc, in pixels.
 * @param a0 arc starting angle
 * @param a1 arc ending angle
 */
void EVE_CoCmd_arc(EVE_HalContext *phost, int16_t x, int16_t y, int16_t r0, uint16_t r1, uint16_t a0, uint16_t a1);

/**
 * @brief Send CMD_GRADIENT
 *
 * @param phost Pointer to Hal context
 * @param x0 x-coordinate of point 0, in pixels
 * @param y0 y-coordinate of point 0, in pixels
 * @param rgb0 Color of point 0, as a 24-bit RGB number
 * @param x1 x-coordinate of point 1, in pixels
 * @param y1 y-coordinate of point 1, in pixels
 * @param rgb1 Color of point 1
 */
void EVE_CoCmd_gradient(EVE_HalContext *phost, int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1);

/**
 * @brief Send CMD_GRADIENTA
 *
 * @param phost Pointer to Hal context
 * @param x0 x-coordinate of point 0, in pixels
 * @param y0 y-coordinate of point 0, in pixels
 * @param argb0 Color of point 0, as a 24-bit RGB number
 * @param x1 x-coordinate of point 1, in pixels
 * @param y1 y-coordinate of point 1, in pixels
 * @param argb1 Color of point 1
 */
void EVE_CoCmd_gradientA(EVE_HalContext *phost, int16_t x0, int16_t y0, uint32_t argb0, int16_t x1, int16_t y1, uint32_t argb1);

/**
 * @brief Send CMD_CGRADIENT
 *
 * @param phost Pointer to Hal context
 * @param shape gradient shape
 * @param x x-coordinate of top-left of rectangle, in pixels
 * @param y y-coordinate of top-left of rectangle, in pixels
 * @param w width of rectangle in pixels
 * @param h height of rectangle in pixels
 * @param argb0 Outer color
 * @param argb1 Inner color
 */
void EVE_CoCmd_cgradient(EVE_HalContext *phost, uint32_t shape, int16_t x, int16_t y, int16_t w, uint16_t h, uint32_t argb0, uint32_t argb1);

/**
 * @brief Send CMD_GLOW
 *
 * @param phost Pointer to Hal context
 * @param x x-coordinate of top-left of rectangle, in pixels
 * @param y y-coordinate of top-left of rectangle, in pixels
 * @param w width of rectangle in pixels
 * @param h height of rectangle in pixels
 */
void EVE_CoCmd_glow(EVE_HalContext *phost, int16_t x, int16_t y, int16_t w, uint16_t h);

/**********************
 *   INLINE FUNCTIONS
 **********************/

/** @brief Write a display list instruction.
 * \n Example: EVE_CoCmd_dl(DISPLAY());
 *
 * @param phost Pointer to Hal context
 * @param dl display list instruction
 */
static inline void EVE_CoCmd_dl(EVE_HalContext *phost, uint32_t dl)
{
    EVE_CoCmd_d(phost, dl);
}

/**********************************************************************
***********************************************************************
**********************************************************************/
#define TO_UPPERCASE_a A
#define TO_UPPERCASE_b B
#define TO_UPPERCASE_c C
#define TO_UPPERCASE_d D
#define TO_UPPERCASE_e E
#define TO_UPPERCASE_f F
#define TO_UPPERCASE_g G
#define TO_UPPERCASE_h H
#define TO_UPPERCASE_i I
#define TO_UPPERCASE_j J
#define TO_UPPERCASE_k K
#define TO_UPPERCASE_l L
#define TO_UPPERCASE_m M
#define TO_UPPERCASE_n N
#define TO_UPPERCASE_o O
#define TO_UPPERCASE_p P
#define TO_UPPERCASE_q Q
#define TO_UPPERCASE_r R
#define TO_UPPERCASE_s S
#define TO_UPPERCASE_t T
#define TO_UPPERCASE_u U
#define TO_UPPERCASE_v V
#define TO_UPPERCASE_w W
#define TO_UPPERCASE_x X
#define TO_UPPERCASE_y Y
#define TO_UPPERCASE_z Z

#define CONCAT(a, b) a##b
#define EXPAND_CONCAT(a, b) CONCAT(a, b)

#define STRINGIFY(x) #x
#define EVE_CMD(x, y) _Pragma(STRINGIFY(define x y))

#define EVE_CoCmd_regwrite(phost, ...) EVE_CoCmd_ddd(phost, CMD_REGWRITE, __VA_ARGS__)
#define EVE_CoCmd_regread(phost, ...) EVE_CoCmd_dd(phost, CMD_REGREAD, __VA_ARGS__)
#define EVE_CoCmd_graphicsfinish(phost, ...) EVE_CoCmd_d(phost, CMD_GRAPHICSFINISH)

#endif /* EVE_CO_CMD__H */

/* end of file */
