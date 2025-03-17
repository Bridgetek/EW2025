/**
 * @file FlashHelper.c
 * @brief Eve's connected flash helper functions
 *
 * @author Tuan Nguyen <tuan.nguyen@brtchip.com>
 *
 * @date 2019
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

#include "EVE_FlashHelper.h"
#include "Gpu_Hal.h"

#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Writes the given data to flash.
 * If the data matches the existing contents of flash, nothing is done.
 * Otherwise the flash is erased in 4K units, and the data is written.
 *
 * dest_flash: destination address in flash memory. Must be 4096-byte aligned
 * src_ram: source data in main memory. Must be 4-byte aligned
 * num: number of bytes to write, should be multiple of 4096, otherwise, dummy data will be padded
 *
 * @param phost Pointer to Hal context
 * @param dest_flash Destination on flash
 * @param src_ram Source in RAM_G
 * @param num Numbber of byte to write
 * @return Flash_Cmd_Status_t
 */
Flash_Cmd_Status_t FlashHelper_Update(EVE_HalContext* phost, uint32_t dest_flash, uint32_t src_ram, uint32_t num)
{
	uint32_t last_chunk = (num % 4096); /* must be multiple of 4096. Cut off the extended data */

	if ((dest_flash % FLASH_UPDATE_ALIGN_BYTE != 0) || ((src_ram % 4) != 0)) /* Check aligned address */
	{
		return FLASH_CMD_ALIGNED_ERR;
	}

	if (num < FLASH_UPDATE_ALIGN_BYTE)
	{
		EVE_CoCmd_flashUpdate(phost, dest_flash, src_ram, FLASH_UPDATE_ALIGN_BYTE);
		EVE_Cmd_waitFlush(phost);
	}
	else if (last_chunk == 0) /* num is multiple of 4k */
	{
		EVE_CoCmd_flashUpdate(phost, dest_flash, src_ram, num);
		EVE_Cmd_waitFlush(phost);
	}
	else /* num is not fit in multiple of 4k */
	{
		EVE_CoCmd_flashUpdate(phost, dest_flash, src_ram, num - last_chunk);
		EVE_Cmd_waitFlush(phost);

		/* 4k is quite big for allocating new stack/heap data. So reuse the pointer and write dummy data to flash */
		EVE_CoCmd_flashUpdate(phost, dest_flash + num - last_chunk, src_ram + num - last_chunk, FLASH_UPDATE_ALIGN_BYTE);
		EVE_Cmd_waitFlush(phost);
	}
	return FLASH_CMD_SUCCESS;
}
#endif

#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Read data from flash to array
 * dest_ram: address in ram where the flash copy data to
 * src_flash: source address in flash memory. Must be 64-byte aligned. From 0 to 64*1024 for 64MB flash
 * num: number of bytes would be read
 * read_data: pointer to user read data
 *
 * @param phost Pointer to Hal context
 * @param dest_ram Destination on RAM_G
 * @param src_flash Source in flash
 * @param num Numbber of byte to read
 * @param read_data Buffer to get data
 * @return Flash_Cmd_Status_t
 */
Flash_Cmd_Status_t FlashHelper_Read(EVE_HalContext* phost, uint32_t dest_ram, uint32_t src_flash, uint32_t num, uint8_t *read_data)
{
	num = num - (num % 4); /* Only read lesser or equal aligned bytes */

	if ((src_flash % FLASH_READ_ALIGN_BYTE != 0) || ((dest_ram % 4) != 0)) /* Check aligned address */
	{
		return FLASH_CMD_ALIGNED_ERR;
	}

	EVE_CoCmd_flashRead(phost, dest_ram, src_flash, num);
	EVE_Cmd_waitFlush(phost);

	EVE_Hal_rdMem(phost, read_data, dest_ram, num);
	return FLASH_CMD_SUCCESS;
}
#endif

#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Erase entire flash
 *
 * @param phost Pointer to Hal context
 * @return ft_void_t
 */
ft_void_t FlashHelper_Erase(EVE_HalContext* phost)
{
	EVE_CoCmd_flashErase(phost);
	EVE_Cmd_waitFlush(phost);
}
#endif

#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Flash state/status
 *
 * @param phost Pointer to Hal context
 * @return uint8_t
 */
uint8_t FlashHelper_GetState(EVE_HalContext* phost)
{
	return EVE_Hal_rd8(phost, REG_FLASH_STATUS);
}
#endif

#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Clears the graphics system's flash cache. It should be executed after modifying graphics data in flash, otherwise bitmaps from flash may render "stale" data.
 * This command must be executed when the display list is in use, immediately after a CMD SWAP command.
 *
 * @param phost Pointer to Hal context
 * @return ft_void_t
 */
ft_void_t FlashHelper_ClearCache(EVE_HalContext* phost)
{
	EVE_CoCmd_clearCache(phost);
	EVE_Cmd_waitFlush(phost);
}
#endif

/*

*/
#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Write data to flash, and align byte if needed.
 * Note:
 * - Destination flash address must be virgin (not used before). Otherwise, users have to perform flash erase before using.
 * - Destination address must be 256-byte aligned.
 * - Automatically padding 0xFF to non-aligned num.
 *
 * @param phost Pointer to Hal context
 * @param dest_flash Destination on flash
 * @param num Number of bytes to write
 * @param write_data Data buffer to write
 * @return Flash_Cmd_Status_t
 */
Flash_Cmd_Status_t FlashHelper_Write(EVE_HalContext* phost, uint32_t dest_flash, uint32_t num, const uint8_t* write_data)
{
	uint8_t padding_arr[FLASH_WRITE_ALIGN_BYTE]; /* write_data must be 256-byte aligned */
	uint32_t aligned_length = num % FLASH_WRITE_ALIGN_BYTE;

	if (dest_flash % FLASH_WRITE_ALIGN_BYTE != 0) /* Check aligned address */
	{
		return FLASH_CMD_ALIGNED_ERR;
	}

	if (aligned_length == 0) /* write_data is already aligned */
	{
		FlashHelper_flashWriteExt(phost, dest_flash, num, write_data);
		EVE_Cmd_waitFlush(phost);
	}
	else
	{
		/* Write first aligned chunks of write_data */
		if (num - aligned_length > 0)
		{
			FlashHelper_flashWriteExt(phost, dest_flash, num - aligned_length, write_data);
			EVE_Cmd_waitFlush(phost);
		}
		/* Write the rest write_data */
		write_data = write_data + num - aligned_length;
		for (uint32_t i = 0; i < FLASH_WRITE_ALIGN_BYTE; i++)
		{
			if (i < aligned_length)
			{
				padding_arr[i] = *write_data++;
			}
			else
			{
				padding_arr[i] = 0xFF; /* Should use 0xFF instead of 0x00 to avoid writing overhead */
			}
		}
		FlashHelper_flashWriteExt(phost, dest_flash + num - aligned_length, FLASH_WRITE_ALIGN_BYTE, padding_arr);
		EVE_Cmd_waitFlush(phost);
	}
	return FLASH_CMD_SUCCESS;
}
#endif

/*

*/
#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Switch to other flash state
 * Error code:
 * - 0x0    command succeeds
 * - 0xffff command fails (invalid transition state)
 * - 0xe001 flash is not attached
 * - 0xe002 no header detected in sector 0 - is flash blank?
 * - 0xe003 sector 0 data failed integrity check
 * - 0xe004 device/blob mismatch - was correct blob loaded?
 * - 0xe005 failed full-speed test - check board wiring
 *
 * @param phost Pointer to Hal context
 * @param nextState New state to switch to
 * @return uint32_t
 */
uint32_t FlashHelper_SwitchState(EVE_HalContext* phost, uint8_t nextState)
{
	uint32_t ret = 0;
	//uint8_t read_data[CMD_FIFO_SIZE]; Debug only
	uint8_t curr_flash_state = EVE_Hal_rd8(phost, REG_FLASH_STATUS);
	uint16_t ret_addr = 0;
	if (curr_flash_state == nextState) {
		return ret;
	}

	//Only handle if nextState is diff
	if (FLASH_STATUS_DETACHED == nextState)
	{
		EVE_CoCmd_flashDetach(phost);
		EVE_Cmd_waitFlush(phost);
	}
	else if (FLASH_STATUS_BASIC == nextState)
	{
		if (FLASH_STATUS_FULL == curr_flash_state)
		{
			do
			{
				EVE_CoCmd_flashDetach(phost);
				EVE_Cmd_waitFlush(phost);
			} while (FLASH_STATUS_DETACHED != EVE_Hal_rd8(phost, REG_FLASH_STATUS));
		}
		EVE_CoCmd_flashAttach(phost);
		EVE_Cmd_waitFlush(phost);
	}
	else if (FLASH_STATUS_FULL == nextState)
	{
		if (FLASH_STATUS_BASIC != curr_flash_state)
		{
			do
			{
				EVE_CoCmd_flashAttach(phost);
				EVE_Cmd_waitFlush(phost);
			} while (FLASH_STATUS_BASIC != EVE_Hal_rd8(phost, REG_FLASH_STATUS));
		}
		EVE_CoCmd_flashFast(phost, 0);
		EVE_Cmd_waitFlush(phost);

		/* Read the return code in CMD_BUFFER */
		ret_addr = (EVE_Cmd_wp(phost) - 4) & FIFO_SIZE_MASK;
		ret_addr = (ret_addr + 3) & FIFO_BYTE_ALIGNMENT_MASK; //4 byte alignment

		ret = EVE_Hal_rd32(phost, RAM_CMD + ret_addr);
	}
	else
	{
		ret = 0xffff;
	}

	return ret;
}
#endif

#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Switch flash into full mode
 *
 * @param phost Pointer to Hal context
 * @return uint32_t
 */
uint32_t FlashHelper_SwitchFullMode(EVE_HalContext* phost)
{
	uint8_t val;
	/* Try detaching and attaching the flash followed by fast mdoe */
	EVE_CoCmd_flashDetach(phost);
	EVE_Cmd_waitFlush(phost);
	val = EVE_Hal_rd8(phost, REG_FLASH_STATUS);

	if (FLASH_STATUS_DETACHED != val)
	{
		printf("Error, Flash is not able to detatch %d\n", val);
		return 0;
	}

	EVE_CoCmd_flashAttach(phost);
	EVE_Cmd_waitFlush(phost);
	val = EVE_Hal_rd8(phost, REG_FLASH_STATUS);

	if (FLASH_STATUS_BASIC != val)
	{
		printf("Error, Flash is not able to attach %d\n", val);
		return 0;
	}

	EVE_CoCmd_flashFast(phost, 0);
	EVE_Cmd_waitFlush(phost);
	val = EVE_Hal_rd8(phost, REG_FLASH_STATUS);

	if (FLASH_STATUS_FULL != val)
	{
		printf("Error, Flash is not able to get into full mode, status: %d\n", val);
		return 0;
	}
	return 1;
}
#endif

#ifdef EVE_FLASH_AVAILABLE
/**
 * @brief Write data to flash. Constraints:
 * - Destination flash address must be virgin (not used before)
 * - data array must be aligned 256-bit
 *
 * @param phost Pointer to Hal context
 * @param dest Destination on flash
 * @param num Number of byte to write
 * @param data Data buffer to write
 * @return ft_void_t
 */
ft_void_t FlashHelper_flashWriteExt(EVE_HalContext* phost, uint32_t dest, uint32_t num, const uint8_t* data)
{
	uint32_t send_data32 = 0;

	EVE_Cmd_startFunc(phost);
	EVE_Cmd_wr32(phost, CMD_FLASHWRITE);
	EVE_Cmd_wr32(phost, dest);
	EVE_Cmd_wr32(phost, num);
	for (uint32_t i = 0; i < num; i = i + 4)
	{
		/* Pack 4 bytes into a 32-bit data each sending package */
		send_data32 = *data++;
		send_data32 |= (*data++) << 8;
		send_data32 |= (*data++) << 16;
		send_data32 |= (*data++) << 24;
		EVE_Cmd_wr32(phost, send_data32);
	}
	EVE_Cmd_endFunc(phost);
}
#endif


/* Nothing beyond this */
