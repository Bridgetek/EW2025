/**
 * @file FlashHelper.h
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

#ifndef FLASHHELPER_H_
#define FLASHHELPER_H_

#include "FT_Gpu_Hal.h"
#include "Gpu_Hal.h"
#include "EVE_Platform.h"

#include "Gpu_CoCmd.h"

Flash_Cmd_Status_t FlashHelper_Update(EVE_HalContext *phost, uint32_t dest_flash, uint32_t src_ram, uint32_t num);
Flash_Cmd_Status_t FlashHelper_Read(EVE_HalContext *phost, uint32_t dest_ram, uint32_t src_flash, uint32_t num, uint8_t *read_data);
ft_void_t FlashHelper_Erase(EVE_HalContext *phost);
uint8_t FlashHelper_GetState(EVE_HalContext *phost);
ft_void_t FlashHelper_ClearCache(EVE_HalContext *phost);
Flash_Cmd_Status_t FlashHelper_Write(EVE_HalContext *phost, uint32_t dest_flash, uint32_t num, const uint8_t *write_data);
uint32_t FlashHelper_SwitchState(EVE_HalContext *phost, uint8_t nextState);
uint32_t FlashHelper_SwitchFullMode(EVE_HalContext *phost);
ft_void_t FlashHelper_flashWriteExt(EVE_HalContext *phost, uint32_t dest, uint32_t num, const uint8_t *data);
ft_void_t Ft_QueueCmd(uint32_t cmd);
ft_void_t Ft_QueueString(const char *s);
ft_void_t Ft_FlushCmd(EVE_HalContext *phost);
ft_void_t Ft_Gpu_CoCmd_FlashWriteExt(EVE_HalContext *phost, uint32_t dest, uint32_t num, const uint8_t *data);

int32_t FlashHelper_GetSizeMB(Gpu_Hal_Context_t* host);

#endif /* FLASHHELPER_H_ */
/* end of file */
