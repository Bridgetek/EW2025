/**
 * @file EVE_Hal.c
 * @brief Eve_Hal framework APIs
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

#include "EVE_Hal.h"
#include "EVE_HalImpl.h"

#include <string.h>

EVE_HalPlatform g_HalPlatform;
/** @name INIT */
///@{

/**
 * @brief Eve_Hal framework initialization
 *
 * @return EVE_HalPlatform* Pointer to EVE_HalPlatform struct
 */
EVE_HalPlatform *EVE_Hal_initialize()
{
	EVE_Mcu_initialize();
	EVE_Millis_initialize();
	EVE_HalImpl_initialize();
	return &g_HalPlatform;
}

/**
 * @brief Close Eve_Hal framework
 *
 */
void EVE_Hal_release()
{
	eve_assert_ex(g_HalPlatform.OpenedDevices == 0, "HAL context still open\n");
	EVE_HalImpl_release();
	EVE_Millis_release();
	EVE_Mcu_release();
	memset(&g_HalPlatform, 0, sizeof(EVE_HalPlatform));
}

/**
 * @brief Setup default parameters for Eve_Hal framework
 *
 * @param parameters Pointer to EVE_HalParameters
 */
void EVE_Hal_defaults(EVE_HalParameters *parameters)
{
	EVE_Hal_defaultsEx(parameters, -1);
}

/**
 * @brief Setup default parameters for Eve_Hal framework
 *
 * @param parameters Pointer to EVE_HalParameters
 * @param deviceIdx
 */
void EVE_Hal_defaultsEx(EVE_HalParameters *parameters, size_t deviceIdx)
{
	memset(parameters, 0, sizeof(EVE_HalParameters));
	eve_assert_do(EVE_HalImpl_defaults(parameters, deviceIdx));
}

/**
 * @brief Open the Eve_Hal framework
 *
 * @param phost Pointer to Hal context
 * @param parameters Pointer to EVE_HalParameters
 * @return true True if ok
 * @return false False if error
 */
bool EVE_Hal_open(EVE_HalContext *phost, const EVE_HalParameters *parameters)
{
	memset(phost, 0, sizeof(EVE_HalContext));
	phost->UserContext = parameters->UserContext;
	phost->CbCmdWait = parameters->CbCmdWait;
	return EVE_HalImpl_open(phost, parameters);
}

/**
 * @brief Close the Eve_Hal framework
 *
 * @param phost Pointer to Hal context
 */
void EVE_Hal_close(EVE_HalContext *phost)
{
	if (phost->Status == EVE_STATUS_CLOSED)
	{
		eve_printf_debug("Attempt to close HAL context that is already closed\n");
		return;
	}

#ifdef EVE_SUPPORT_MEDIAFIFO
	EVE_Util_closeFile(phost);
#endif
	EVE_HalImpl_close(phost);
	memset(phost, 0, sizeof(EVE_HalContext));
}

/**
 * @brief Idle handler for Eve_Hal framework
 *
 * @param phost Pointer to Hal context
 */
void EVE_Hal_idle(EVE_HalContext *phost)
{
	EVE_HalImpl_idle(phost);
}

///@}

/*********************
** TRANSFER HELPERS **
*********************/

/** @name TRANSFER HELPERS */
///@{

/**
 * @brief Read 8 bits from Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be read
 * @return uint8_t Data from Coprocessor
 */
uint8_t EVE_Hal_rd8(EVE_HalContext *phost, uint32_t addr)
{
	uint8_t value;
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_READ, addr);
	value = EVE_Hal_transfer8(phost, 0);
	EVE_Hal_endTransfer(phost);
	return value;
}

/**
 * @brief Read 2 bytes from Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be read
 * @return uint16_t Data from Coprocessor
 */
uint16_t EVE_Hal_rd16(EVE_HalContext *phost, uint32_t addr)
{
	uint16_t value;
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_READ, addr);
	value = EVE_Hal_transfer16(phost, 0);
	EVE_Hal_endTransfer(phost);
	return value;
}

/**
 * @brief Read 4 bytes from Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be read
 * @return uint16_t Data from Coprocessor
 */
uint32_t EVE_Hal_rd32(EVE_HalContext *phost, uint32_t addr)
{
	uint32_t value;
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_READ, addr);
	value = EVE_Hal_transfer32(phost, 0);
	EVE_Hal_endTransfer(phost);
	return value;
}

/**
 * @brief Read a block data from Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param result Buffer where data write to
 * @param addr Address to bbe read
 * @param size Size to be read
 */
void EVE_Hal_rdMem(EVE_HalContext *phost, uint8_t *result, uint32_t addr, uint32_t size)
{
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_READ, addr);
	EVE_Hal_transferMem(phost, result, NULL, size);
	EVE_Hal_endTransfer(phost);
}

/**
 * @brief Write 1 bytes to Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be write
 * @param v Value to write
 */
void EVE_Hal_wr8(EVE_HalContext *phost, uint32_t addr, uint8_t v)
{
	return EVE_Hal_wr32(phost, addr, (uint32_t)v); // Work arround for BT820

	EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, addr);
	EVE_Hal_transfer32(phost, v);
	EVE_Hal_endTransfer(phost);
}

/**
 * @brief Write 2 bytes to Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be write
 * @param v Value to write
 */
void EVE_Hal_wr16(EVE_HalContext *phost, uint32_t addr, uint16_t v)
{
	return EVE_Hal_wr32(phost, addr, (uint32_t)v); // Work arround for BT820

	EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, addr);
	EVE_Hal_transfer32(phost, v);
	EVE_Hal_endTransfer(phost);
}

/**
 * @brief Write 4 bytes to Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be write
 * @param v Value to write
 */
void EVE_Hal_wr32(EVE_HalContext *phost, uint32_t addr, uint32_t v)
{
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, addr);
	EVE_Hal_transfer32(phost, v);
	EVE_Hal_endTransfer(phost);
}

/**
 * @brief Write a buffer to Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be write
 * @param buffer Data to be write
 * @param size Size of buffer
 */
void EVE_Hal_wrMem(EVE_HalContext *phost, uint32_t addr, const uint8_t *buffer, uint32_t size)
{
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, addr);
	EVE_Hal_transferMem(phost, NULL, buffer, size);
	EVE_Hal_endTransfer(phost);
}

/**
 * @brief Write a buffer in ProgMem to Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be write
 * @param buffer Data to be write
 * @param size Size of buffer
 */
void EVE_Hal_wrProgMem(EVE_HalContext *phost, uint32_t addr, eve_progmem_const uint8_t *buffer, uint32_t size)
{
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, addr);
	EVE_Hal_transferProgMem(phost, NULL, buffer, size);
	EVE_Hal_endTransfer(phost);
}

/**
 * @brief Write a string to Coprocessor's memory
 *
 * @param phost Pointer to Hal context
 * @param addr Address to be write
 * @param str String to be write
 * @param index Start postion in the string
 * @param size Size of the string
 * @param padMask Padding mask
 */
void EVE_Hal_wrString(EVE_HalContext *phost, uint32_t addr, const char *str, uint32_t index, uint32_t size, uint32_t padMask)
{
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, addr);
	EVE_Hal_transferString(phost, str, index, size, padMask);
	EVE_Hal_endTransfer(phost);
}
///@}

/****************
** SPI command **
****************/

/** @name SPI command */
///@{

/**
 * @brief set power state
 * @param state EVE power state
 */
void EVE_Hal_SPICmd_pwr_state(EVE_HalContext *phost, EVE_PWR_STATE_T state)
{
	uint8_t CmdBuf[5] = {0};

	switch (state)
	{
	case EVE_PWR_STATE_ACTIVE:
		break;
	case EVE_PWR_STATE_STANDBY:
		CmdBuf[0] = 0xFF;
		CmdBuf[1] = 0xE0;
		break;
	case EVE_PWR_STATE_SLEEP:
		CmdBuf[0] = 0xFF;
		CmdBuf[1] = 0xE1;
		break;
	case EVE_PWR_STATE_POWERDOWN:
		CmdBuf[0] = 0xFF;
		CmdBuf[1] = 0xE2;
		break;
	default:
		break;
	}
	EVE_Hal_spiCommand(phost, CmdBuf);
}

/**
 * @brief Program System PLL frequency
 * @param val
 * -------------------------------------------------
 * |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 * | CKS |             NS                          |
 * -------------------------------------------------
 */
void EVE_Hal_SPICmd_setpllsp1(EVE_HalContext *phost, EVE_CMD_SYS_PLL_FREQ val)
{
	uint8_t CmdBuf[5] = {0xFF, 0xE4, 0x00, 0x00, 0x00};
	CmdBuf[2] = (val.PLL_CKS << 7) | val.PLL_NS;
	EVE_Hal_spiCommand(phost, CmdBuf);
}

/**
 * @brief Set system clock divider
 * @param val
 * -------------------------------------------------
 * |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 * | rev |  0  |  0  |  1  |          freq         |
 * -------------------------------------------------
 */
void EVE_Hal_SPICmd_sysclkdiv(EVE_HalContext *phost, EVE_CMD_SYS_CLK_DIV val)
{
	uint8_t CmdBuf[5] = {0xFF, 0xE6, 0x00, 0x00, 0x00};
	CmdBuf[2] = (val.SYSPLL_CPS << 4) | val.freq;
	EVE_Hal_spiCommand(phost, CmdBuf);
}

/**
 * @brief Set EVE boot sequence configuration
 * @param val
 * ----------------------------------------------------
 * |  7  |  6  |  5  |  4     |  3  |  2  |  1  |  0  |
 * | DDR | JT  |audio|watchdog| rev |      source     |
 * ----------------------------------------------------
 */
void EVE_Hal_SPICmd_setbootcfg(EVE_HalContext *phost, EVE_CMD_BOOT_CFG val)
{
	uint8_t CmdBuf[5] = {0xFF, 0xE8, 0x00, 0x00, 0x00};
	CmdBuf[2] = (val.ddr << 7) | (val.JT << 6) | (val.audio << 5) | (val.watchdog << 4) | val.source;
	EVE_Hal_spiCommand(phost, CmdBuf);
}

/**
 * @brief controls the handling of the EVE boot sequence configuration
 * @param val
 * ----------------------------------------------------
 * |  7  |  6    |  5  |  4  |  3  |  2  |  1  |  0   |
 * |boot |DDRtype|GPREG|           rev         |enable|
 * ----------------------------------------------------
 */
void EVE_Hal_SPICmd_bootcfgen(EVE_HalContext *phost, EVE_CMD_BOOT_CFG_EN val)
{
	uint8_t CmdBuf[5] = {0xFF, 0xE9, 0x00, 0x00, 0x00};
	CmdBuf[2] = (val.boot << 7) | (val.DDRtype << 6) | (val.GPREG << 5) | val.enable;
	EVE_Hal_spiCommand(phost, CmdBuf);
}

/**
 * @brief programs the DDR DRAM type
 * @param val
 * -------------------------------------------------
 * |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 * |      speed      |   type    |      size       |
 * -------------------------------------------------
 */
void EVE_Hal_SPICmd_setddrtype(EVE_HalContext *phost, EVE_CMD_DDR_TYPE val)
{
	uint8_t CmdBuf[5] = {0xFF, 0xEB, 0x00, 0x00, 0x00};
	CmdBuf[2] = (val.speed << 5) | (val.type << 3) | val.size;
	EVE_Hal_spiCommand(phost, CmdBuf);
}

/**
 * @brief program a 32-bit general purpose register
 * @param val
 * -------------------------------------------------
 * |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 * |              rev                        |mode |
 * -------------------------------------------------
 */
void EVE_Hal_SPICmd_setgpreg(EVE_HalContext *phost, EVE_CMD_GPREG val)
{
	uint8_t CmdBuf[5] = {0xFF, 0xEC, 0x00, 0x00, 0x00};
	CmdBuf[2] = val.mode;
	EVE_Hal_spiCommand(phost, CmdBuf);
}
///@}

/**********
** DEBUG **
**********/

/** @name DEBUG */
///@{

/**
 * @brief Display a fullscreen debug message using TEXT8X8. Uses the back of RAM_G.
 *
 * @param phost Pointer to Hal context
 * @param str Error message to show
 * @param size Size of the message
 */
void EVE_Hal_displayMessage(EVE_HalContext *phost, const char *str, uint16_t size)
{
	uint32_t round = ((size + 31U) & ~31U);
	uint32_t addr = RAM_G + phost->DDR_RamSize - round;
	uint32_t dl = 0;

	/* Abuse back of RAM_G to store error */
	/* May invalidate user data... */
	EVE_Hal_wrMem(phost, addr, (uint8_t *)str, size);

	/* Empty remaining space after text */
	EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, addr + size);
	for (uint32_t i = size; i < round; ++i)
		EVE_Hal_transfer8(phost, 0);
	EVE_Hal_endTransfer(phost);

	/* Generate bluescreen */
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), CLEAR_COLOR_RGB(0x00, 0x20, 0x40));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), CLEAR(1, 1, 1));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BITMAP_HANDLE(15)); /* Scratch handle will reset anyway after reset */
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BITMAP_SOURCE(addr));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BITMAP_SIZE_H(0, 0));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BITMAP_SIZE(NEAREST, BORDER, BORDER, 256, (round >> 2)));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BITMAP_LAYOUT_H(0, 0));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BITMAP_LAYOUT(TEXT8X8, 32, (round >> 2)));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BEGIN(BITMAPS));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), VERTEX2II(32, 32, 15, 0));
#if ENABLE_ERR_REPORT
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), BITMAP_SOURCE(RAM_ERR_REPORT));
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), VERTEX2II(32, 96, 15, 0));
#endif
	EVE_Hal_wr32(phost, RAM_DL + ((dl++) << 2), DISPLAY());
	EVE_Hal_wr32(phost, REG_DLSWAP, DLSWAP_FRAME);
}
///@}

/* end of file */
