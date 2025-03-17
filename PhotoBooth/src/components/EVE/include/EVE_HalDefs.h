/**
 * @file EVE_HalDefs.h
 * @brief Eve_Hal framework APIs
 *
 *  This file defines the generic APIs of phost access layer for the BT820 or EVE compatible silicon.
 *  Application shall access BT820 or EVE resources over these APIs,regardless of I2C or SPI protocol.
 *  In addition, there are some helper functions defined for FT800 coprocessor engine as well as phost commands.
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

#ifndef EVE_HAL_DEFS__H
#define EVE_HAL_DEFS__H
/*************
** INCLUDES **
*************/
#include "EVE_GpuTypes.h"

#if EVE_ENABLE_FATFS
#include "ff.h"
#endif
/***********
** MARCOS **
***********/

#define RAM_ERR_REPORT_MAX 128

/*************
** TYPEDEFS **
*************/

typedef enum EVE_STATUS_T
{
	EVE_STATUS_CLOSED = 0, /**< 0 */
	EVE_STATUS_OPENED,
	EVE_STATUS_READING,
	EVE_STATUS_WRITING,
	EVE_STATUS_ERROR,
} EVE_STATUS_T;

typedef enum EVE_TRANSFER_T
{
	EVE_TRANSFER_NONE = 0, /**< 0 */
	EVE_TRANSFER_READ,
	EVE_TRANSFER_WRITE,
} EVE_TRANSFER_T;

typedef enum EVE_CHIPID_T
{
	EVE_CHIPID_BT820 = EVE_BT820, /**< EVE_BT820 */
} EVE_CHIPID_T;

typedef enum EVE_HOST_T
{
	EVE_HOST_UNKNOWN = 0, /**< 0 */
	EVE_HOST_BT8XXEMU,
	EVE_HOST_FT4222,
	EVE_HOST_MPSSE,
	EVE_HOST_EMBEDDED,

	EVE_HOST_NB
} EVE_HOST_T;

typedef enum EVE_PWR_STATE_T
{
	EVE_PWR_STATE_ACTIVE = 0, /**< 0 */
	EVE_PWR_STATE_STANDBY,
	EVE_PWR_STATE_SLEEP,
	EVE_PWR_STATE_POWERDOWN
} EVE_PWR_STATE_T;

/** SETPLLSP1 */
typedef struct EVE_CMD_SYS_PLL_FREQ
{
	uint8_t PLL_NS : 7;	 /**< PLL frequency multiplier factor.Default : 15 */
	uint8_t PLL_CKS : 1; /**< PLL range */
} EVE_CMD_SYS_PLL_FREQ;

/** SYSCLKDIV */
typedef struct EVE_CMD_SYS_CLK_DIV
{
	uint8_t freq : 4;		/**< system clock division factor. Default: 7 */
	uint8_t SYSPLL_CPS : 3; /**< b'001' */
	uint8_t rev : 1;		/**< reserved */
} EVE_CMD_SYS_CLK_DIV;

/** SETBOOTCFG */
typedef struct EVE_CMD_BOOT_CFG
{
	uint8_t source : 3;	  /**< source field */
	uint8_t rev : 1;	  /**< reserved */
	uint8_t watchdog : 1; /**< 1 - watchdog system will init */
	uint8_t audio : 1;	  /**< 1 - audio system will init */
	uint8_t JT : 1;		  /**< 1 - JT coprocessor will init */
	uint8_t ddr : 1;	  /**< 1 - DDR subsystem will init */
} EVE_CMD_BOOT_CFG;

/** BOOTCFGEN */
typedef struct EVE_CMD_BOOT_CFG_EN
{
	uint8_t enable : 1;	 /**< 1 - allow update of boot control settings; 0 (default) - ignore updates to boot */
	uint8_t rev : 4;	 /**< reserved */
	uint8_t GPREG : 1;	 /**< GPREG_USER_SETTING; 0 - DEFAULT_GPREG */
	uint8_t DDRtype : 1; /**< 1 - DDRTYPE_USER_SETTING; 0 - DEFAULT_DDRTYPE */
	uint8_t boot : 1;	 /**< 1 - BOOT_USER_SETTING; 0 - DEFAULT_BOOTCFG */
} EVE_CMD_BOOT_CFG_EN;

/** SETDDRTYPE - ref: REG_DDR_TYPE */
typedef struct EVE_CMD_DDR_TYPE
{
	uint8_t size : 3;  /**< DDR_SIZEDDR DRAM size */
	uint8_t type : 2;  /**< DDR_TYPE */
	uint8_t speed : 3; /**< DDR_SPEEDGRADE */
} EVE_CMD_DDR_TYPE;

/** SETGPREG - ref: REG_GPREG */
typedef struct EVE_CMD_GPREG
{
	uint8_t mode : 1; /**< SPI master slow mode */
	uint8_t rev : 7;  /**< reserved */
} EVE_CMD_GPREG;

typedef struct EVE_HalContext EVE_HalContext;
typedef bool (*EVE_Callback)(EVE_HalContext *phost);

/** Hook into coprocessor commands. Return 1 to abort the command. Useful for an optimization routine */
typedef int (*EVE_CoCmdHook)(EVE_HalContext *phost, uint32_t cmd, uint32_t state);
/** Hook into coprocessor reset */
typedef void (*EVE_ResetCallback)(EVE_HalContext *phost, bool fault);

typedef struct EVE_DeviceInfo
{
	char DisplayName[256];
	char SerialNumber[16];
	EVE_HOST_T Host;
	bool Opened;

} EVE_DeviceInfo;

/** Hal parameters */
typedef struct EVE_HalParameters
{
	void *UserContext;

	/** Called anytime the code is waiting during CMD write. Return false to abort wait */
	EVE_Callback CbCmdWait;
} EVE_HalParameters;

typedef struct EVE_HalContext
{
	/** Pointer to user context */
	void *UserContext;

	/** Pointer to a support library context (e.g. ESD Framework context) */
	void *LibraryContext;

	/** Called anytime the code is waiting during CMD write. Return false to abort wait */
	EVE_Callback CbCmdWait;
	/** Callback hook called anytime the coprocessor is reset through the EVE_Util interface */
	EVE_ResetCallback CbCoprocessorReset;

#if EVE_CMD_HOOKS
	/* Hook into coprocessor commands. Called when EVE_CoCmd interface is used. Return 1 to abort the command. Useful for an optimization routine */
	EVE_CoCmdHook CoCmdHook;
#endif

	EVE_STATUS_T Status;

	uint8_t PCLK;

	/** @name User space width and height, based on REG_HSIZE, REG_VSIZE */
	///@{
	uint32_t Width;
	uint32_t Height;
	///@}

	/** @name Handles to external context */
	///@{
	EVE_SPI_CHANNELS_T SpiChannels; /**< Variable to contain single/dual/quad channels */
	///@}

	/** @name Buffer cmd smaller than a full cmd command */
	///@{
	uint8_t CmdBuffer[4];
	uint8_t CmdBufferIndex;

	uint16_t CmdSpace; /**< Free space, cached value */
					   ///@}

	/** @name Media FIFO state */
	///@{
#if defined(EVE_SUPPORT_MEDIAFIFO)
	uint32_t MediaFifoAddress;
	uint32_t MediaFifoSize;
#if EVE_ENABLE_FATFS
	FIL LoadFileObj;
#else
	void *LoadFileHandle;
#endif
	ptrdiff_t LoadFileRemaining;
#endif
	///@}

	/** @name Display list optimization and compatibility caches */
	///@{
	uint8_t CoScratchHandle;
	///@}

#if defined(_DEBUG)
	bool DebugMessageVisible;
	uint8_t DebugBackup[RAM_ERR_REPORT_MAX];
#endif

	/** @name Status flags */
	///@{
	bool CmdFunc;	 /**< Flagged while transfer to cmd is kept open */
	bool CmdFault;	 /**< Flagged when coprocessor is in fault mode and needs to be reset */
	bool CmdWaiting; /**< Flagged while waiting for CMD write (to check during any function that may be called by CbCmdWait) */
	///@}

	uint32_t DDR_RamSize; /**< DDR RAM SIZE from REG_RAM_SIZE For BT82X */

} EVE_HalContext;

typedef struct EVE_HalPlatform
{
	uint32_t OpenedDevices;

} EVE_HalPlatform;

/**********************
 *   INLINE FUNCTIONS
 **********************/

/** Include the EVE generation in the chip ID value to simplify feature set comparisons(BT880 support) */
static inline EVE_CHIPID_T EVE_extendedChipId(int chipId)
{
	switch (chipId & 0xFFFF)
	{
	case EVE_BT820 & 0xFFFF:
		return (EVE_CHIPID_T)((chipId & 0xFFFF) | 0x50000);
	default:
		break;
	}
	return (EVE_CHIPID_T)(chipId & 0xFFFF);
}

/** Remove EVE generation from the chip ID */
static inline int EVE_shortChipId(EVE_CHIPID_T chipId)
{
	return chipId & 0xFFFF;
}

static inline int EVE_gen(EVE_CHIPID_T chipId)
{
	switch (chipId)
	{
	case EVE_BT820:
		return EVE5;
	default:
		break;
	}
	return 0;
}

#endif /* #ifndef EVE_HAL_DEFS__H */

/* end of file */