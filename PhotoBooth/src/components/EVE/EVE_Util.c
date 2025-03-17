/**
 * @file EVE_Util.c
 * @brief EVE HAL framework utilities
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
#include <math.h>
#include <string.h>

#define TEST_BOOT_DDR 0

static eve_progmem_const uint8_t c_DlCodeBootup[12] = {
	0, 0, 0, 2,	 // GPU instruction CLEAR_COLOR_RGB: BLUE, GREEN, RED, ID(0x02)
	7, 0, 0, 38, // GPU instruction CLEAR: Tag/Stencil/Color, ID(0x26)
	0, 0, 0, 0,	 // GPU instruction DISPLAY: ID(0x00)
};

/** @note Update `EVE_DISPLAY_T` in `EVE_Util.h` when adding display presets. */
static const uint16_t s_DisplayResolutions[EVE_DISPLAY_NB][4] = {
	/* width, height, refreshRate, hsfWidth */

	{480, 272, 60, 0}, /* EVE_DISPLAY_DEFAULT (values ignored) */

	/* Landscape */
	{1920, 1080, 60, 0}, /* EVE_DISPLAY_FHD_1920x1080_60Hz */
	{1920, 1200, 60, 0}, /* EVE_DISPLAY_WUXGA_1920x1200_60Hz */
};

/**
 * @brief Starts a timeout period.
 *
 * @returns The current time in milliseconds since the Pico started, used as the
 *          start time for the timeout period.
 */
uint32_t EVE_Util_timeout_start()
{
	return EVE_millis() / 1000;
}

/**
 * @brief Check if a timeout period has elapsed
 *
 * @param start_time The time in seconds when the timeout period started
 * @param timeout_ms The timeout period in milliseconds
 * @return true if the timeout has occurred, false otherwise
 */
bool EVE_Util_timeout(uint32_t start_time, uint32_t timeout_ms)
{
	// Get the current time in milliseconds since the Pico started
	uint32_t current_time = EVE_millis() / 1000;

	// Check if the timeout period has elapsed
	if (current_time - start_time >= timeout_ms)
	{
		return true; // Timeout occurred
	}
	return false; // Timeout not yet occurred
}

/**
 * @brief Clear the screen
 *
 * @param phost Pointer to Hal context
 */
void EVE_Util_clearScreen(EVE_HalContext *phost)
{
	EVE_Hal_wrProgMem(phost, RAM_DL, c_DlCodeBootup, sizeof(c_DlCodeBootup));
	EVE_Hal_wr8(phost, REG_DLSWAP, DLSWAP_FRAME);
}

/**
 * @brief Set up default parameters for bootup in EVE HAL framework
 *
 * @param phost  Pointer to Hal context
 * @param bootup Pointer to Bootup parameters
 */
void EVE_Util_bootupDefaults(EVE_HalContext *phost, EVE_BootupParameters *bootup)
{
	int32_t chipId = EVE_CHIPID;
	(void)chipId;

	// Initialize bootup parameters
	memset(bootup, 0, sizeof(EVE_BootupParameters));
	bootup->ExternalOsc = true;

	// Check if the chip ID is BT820
	if (chipId == EVE_BT820)
	{
		bootup->SystemClock = EVE_SYSCLK_72M; /* 72Mhz is default for BT82x */
	}
	else
	{
		/* If the chipid is not known yet, this will reconfigure the system clock during `EVE_Util_bootup`.
		Additional default clock overrides need to be implemented there as well. */
		bootup->SystemClock = EVE_SYSCLK_DEFAULT;
	}

	// Set SPI channel based on configuration
#if defined(ENABLE_SPI_QUAD)
	bootup->SpiChannels = EVE_SPI_QUAD_CHANNEL;
#elif defined(ENABLE_SPI_DUAL)
	bootup->SpiChannels = EVE_SPI_DUAL_CHANNEL;
#else
	bootup->SpiChannels = EVE_SPI_SINGLE_CHANNEL;
#endif
}

#ifndef EVE_HARDCODED_DISPLAY_TIMINGS
#define EVE_HARDCODED_DISPLAY_TIMINGS 1
#endif

/**
 * @brief
 *
 * @param phost Pointer to Hal context
 * @param config
 * @param width
 * @param height
 * @param refreshRate
 * @param hsfWidth
 * @param freq
 * @return true True if successful
 * @return false False if error
 */
static bool configDefaultsEx(EVE_HalContext *phost, EVE_ConfigParameters *config, uint32_t width, uint32_t height, uint32_t refreshRate, uint32_t hsfWidth, uint32_t freq)
{
	/* Only false if the output resolution will be wrong.
	Not affected by unsupported HSF or refreshRate */
	bool supportedResolution = true;

	uint32_t screenWidth;
	uint32_t pixels;
	uint32_t minCycles;
	uint32_t maxRate;
	uint32_t pclk;

	uint32_t cycles;
	uint32_t vcycle;
	uint32_t hcycle;
	uint32_t hoffset;
	uint32_t voffset;
	uint32_t hsync1;
	uint32_t vsync1;

	memset(config, 0, sizeof(EVE_ConfigParameters));

	/*
	Refresh rate is as follows:
	REG_VCYCLE * REG_HCYCLE * REG_PCLK is the number of clocks each frame takes,
	Divide REG_FREQUENCY by this number to get the frame refresh rate in Hz.
	REG_HCYCLE must be higher than REG_HSIZE, REG_VCYCLE must be higher than REG_VSIZE.

	By default, BT820 is at 72MHz.
	User may configure this differently in `EVE_Util_bootup`.

	Clocks per frame by clock frequency at 60Hz:
	- 84MHz: 1400k
	- 72MHz: 1200k
	- 60MHz: 1000k
	- 48MHz: 800k
	- 36MHz: 600k
	- 24MHz: 400k
	*/

	/* Trim oversize displays */
	if (width > 2047)
	{
		eve_printf_debug("Line buffer width %i is too large for this configuration\n", (int)config->Width);
		width = 2047;
		supportedResolution = false;
	}
	if (height > 2047)
	{
		eve_printf_debug("Screen height %i is too large for this configuration\n", (int)config->Height);
		height = 2047;
		supportedResolution = false;
	}

	if (hsfWidth)
	{
		/* Use the screen width if HSF is not supported */
		config->Width = (int16_t)hsfWidth;
		eve_printf_debug("Display config specifies HSF but not supported on this platform\n");
	}
	else
	{
		config->Width = (int16_t)width;
	}
	config->Height = (int16_t)height;

	screenWidth = hsfWidth ? hsfWidth : width; /* Use screen width for calculation */
	pixels = screenWidth * height;

	/* Calculate maximum refresh rate */
	minCycles = pixels + (pixels >> 2); /* pixels * 1.25 */
	maxRate = freq / minCycles;

	/* If the refresh rate is too low, try with tighter settings */
	if (refreshRate > maxRate)
	{
		minCycles = pixels + (pixels >> 3); /* pixels * 1.125 */
		maxRate = freq / minCycles;
	}

	if (maxRate < refreshRate)
	{
		/* Trim unsupported framerate */
		eve_printf_debug("Frame rate limited to %d\n", (unsigned int)maxRate);
		refreshRate = maxRate;
		pclk = 1;
	}
	else
	{
		pclk = maxRate / refreshRate;
	}
	config->PCLK = (uint8_t)pclk;

	/* Change PCLKPol if the right edge of the display is noisy,
	or if the display is showing jagged colors. Values 0 or 1.
	This toggles the polarity of the PCLK. */
	config->PCLKPol = 1; /* non-default */

	/* Approximate an average good setting */
	cycles = freq / (refreshRate * pclk);
	vcycle = height * cycles / pixels;									 /* max vcycle without blanking, on average 1.125x height, or 1.25x */
	vcycle = (vcycle + ((vcycle + height) >> 1) + height + height) >> 2; /* vcycle is 1.5 max vcycle + 2.5 height / 4, so on average 1.046875x height, or 1.09375x */
	hcycle = cycles / vcycle;											 /* cycles div by vcycle, on average 1.07375x, or 1.1425x */
	hoffset = (hcycle - screenWidth) >> 1;
	voffset = (vcycle - height) >> 1;
	hsync1 = hoffset >> 1;
	vsync1 = voffset >> 1;
	hoffset += hsync1;
	voffset += vsync1;
	config->HCycle = (int16_t)hcycle;
	config->HSync0 = 0;
	config->HSync1 = (int16_t)hsync1;
	config->HOffset = (int16_t)hoffset;
	config->VCycle = (int16_t)vcycle;
	config->VSync0 = 0;
	config->VSync1 = (int16_t)vsync1;
	config->VOffset = (int16_t)voffset;

	/* Verify */
	eve_assert(config->HSync1);
	eve_assert(config->HOffset > config->HSync1);
	eve_assert(config->HCycle > config->Width);
	eve_assert((config->HCycle - config->Width) > config->HOffset);
	eve_assert(config->VSync1);
	eve_assert(config->VOffset > config->VSync1);
	eve_assert(config->VCycle > config->Height);
	eve_assert((config->VCycle - config->Height) > config->VOffset);

	return supportedResolution;
}

/**
 * @brief Set default config parameters
 *
 * @param phost Pointer to Hal context
 * @param config
 * @param display
 */
void EVE_Util_configDefaults(EVE_HalContext *phost, EVE_ConfigParameters *config, EVE_DISPLAY_T display)
{
	bool supportedResolution;
	uint32_t freq = EVE_Hal_rd32(phost, REG_FREQUENCY);
	uint32_t width;
	uint32_t height;
	uint32_t refreshRate;
	uint32_t hsfWidth;

	/* NOTE: Update `EVE_DISPLAY_T` in `EVE_Util.h` when adding display presets. */
	if (!display)
	{
		/* Default displays if none was explicitly chosen */
		display = EVE_DISPLAY_WUXGA_1920x1200_60Hz;
	}

	width = s_DisplayResolutions[display][0];
	height = s_DisplayResolutions[display][1];
	refreshRate = s_DisplayResolutions[display][2];
	hsfWidth = s_DisplayResolutions[display][3];
	supportedResolution = configDefaultsEx(phost, config, width, height, refreshRate, hsfWidth, freq);

	/* Known values for specific display models */
	if (display == EVE_DISPLAY_WUXGA_1920x1200_60Hz)
	{
		/*
		BT820 known values:
		REG_FREQUENCY: 72MHz
		Resolution: 1920x1200
		Refresh rate: 60Hz
		*/
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (supportedResolution)
		{
			config->Width = 1920;
			config->Height = 1200;
			config->HCycle = 2100;
			config->HOffset = 80;
			config->HSync0 = 64;
			config->HSync1 = 74;
			config->VCycle = 1245;
			config->VOffset = 25;
			config->VSync0 = 20;
			config->VSync1 = 23;
			config->PCLK = 1;
		}
#endif
		config->PCLKPol = 0;
	}

#ifdef _DEBUG
	eve_printf_debug("Display refresh rate set to %f Hz\n", (float)((double)freq / ((double)config->HCycle * (double)config->VCycle * (double)config->PCLK)));
#endif
}

#define EXTRACT_CHIPID(romChipId) EVE_extendedChipId(((romChipId) >> 16) & 0xFFFF)

/**
 * @brief Bootup Coprocessor
 *
 * @param phost Pointer to Hal context
 * @param bootup
 * @return true True if successful
 * @return false False if error
 */
bool EVE_Util_bootup(EVE_HalContext *phost, EVE_BootupParameters *bootup)
{
	/* IMPORTANT: Do not use EVE_CoCmd functions here, as they can be overridden by hooks */

	const uint32_t expectedChipId = EVE_CHIPID;
	uint8_t engineStatus;
	uint32_t chipId;
	uint8_t id;
	double syspll;
	uint32_t freqbyte;
	EVE_CMD_SYS_CLK_DIV sys_clk_div = {0};
	EVE_CMD_BOOT_CFG boot_cfg = {0};
	EVE_CMD_BOOT_CFG_EN boot_cfg_en = {0};
	EVE_CMD_DDR_TYPE ddr_type = {0};
	EVE_CMD_GPREG gpreg = {0};

	do
	{
		/* EVE will be in SPI Single channel after POR */
		if (!EVE_Hal_powerCycle(phost, true))
			return false;

		boot_cfg_en.boot = 1;
		boot_cfg_en.DDRtype = 1;
		boot_cfg_en.GPREG = 0;
		boot_cfg_en.enable = 1;
		EVE_Hal_SPICmd_bootcfgen(phost, boot_cfg_en);
		boot_cfg.ddr = 1;
		boot_cfg.JT = 1;
		boot_cfg.audio = 1;
		boot_cfg.watchdog = 1;
		boot_cfg.source = 0;
		EVE_Hal_SPICmd_setbootcfg(phost, boot_cfg); // REG_BOOT_CONTROL settings: DDR(0) JT(1) AUD(1) WD(0) reserved source(0)
		ddr_type.speed = 0;							// 1333
		ddr_type.type = 1;
		ddr_type.size = 2;
		EVE_Hal_SPICmd_setddrtype(phost, ddr_type);
		gpreg.mode = 0;
		EVE_Hal_SPICmd_setgpreg(phost, gpreg); // SPI Master slow mode(0)
		boot_cfg_en.boot = 1;
		boot_cfg_en.DDRtype = 1;
		boot_cfg_en.GPREG = 0;
		boot_cfg_en.enable = 0;
		EVE_Hal_SPICmd_bootcfgen(phost, boot_cfg_en); // enable(0)
		syspll = 576000000;
		freqbyte = (uint32_t)((round(syspll / FREQUENCY) - 1));
		sys_clk_div.SYSPLL_CPS = 1;	 // default b'001'
		sys_clk_div.freq = freqbyte; // system clock: 72MHz or 36MHz
		EVE_Hal_SPICmd_sysclkdiv(phost, sys_clk_div);
		EVE_Hal_SPICmd_pwr_state(phost, EVE_PWR_STATE_ACTIVE);
		EVE_sleep(40);

		uint32_t start_time = EVE_Util_timeout_start();
		uint32_t timeout = 3000;
		while (EVE_Hal_rd32(phost, REG_BOOT_STATUS) != 0x522e2e2e) // Polling to indicate EVE is in normal running operation state
		{
			EVE_sleep(10);
			if (EVE_Util_timeout(start_time, timeout))
			{
				eve_printf_debug("Boot status 1 %lx\n", EVE_Hal_rd32(phost, REG_BOOT_STATUS));
				start_time = EVE_Util_timeout_start();
			}
		}
		eve_printf_debug("Boot status 2 %lx \n", EVE_Hal_rd32(phost, REG_BOOT_STATUS));
		chipId = EVE_Hal_rd32(phost, REG_CHIP_ID);
		eve_printf_debug("chipID %lx \n", chipId);

		while (EXTRACT_CHIPID(chipId) != EVE_BT820)
		{
			eve_printf_debug("EVE REG_CHIP_ID after wake up %lx\n", (unsigned long)chipId);

			EVE_sleep(20);
			if (phost->CbCmdWait && !phost->CbCmdWait(phost))
				return false;

			chipId = EVE_Hal_rd32(phost, REG_CHIP_ID);
		}
	} while (!chipId);

	/* Validate chip ID to ensure the correct HAL is used */
	/* ROM_CHIPID is valid across all EVE devices */
	if (expectedChipId && EXTRACT_CHIPID(chipId) != expectedChipId)
		eve_printf_debug("Mismatching EVE chip id %lx, expect model %lx\n", (unsigned long)((chipId >> 8) & 0xFF) | ((chipId & 0xFF) << 8), (unsigned long)expectedChipId);
	eve_printf_debug("EVE chip id %lx %lx.%lx (EVE gen %i)\n", (unsigned long)EVE_shortChipId(EXTRACT_CHIPID(chipId)), (unsigned long)((chipId >> 16) & 0xFF), (unsigned long)((chipId >> 24) & 0xFF), EVE_gen(EXTRACT_CHIPID(chipId)));

	/* Read Register ID to check if EVE is ready. */
	while ((id = EVE_Hal_rd8(phost, REG_ID)) != 0x7C)
	{
		eve_printf_debug("EVE register ID after wake up %x\n", (unsigned int)id);

		EVE_sleep(20);
		if (phost->CbCmdWait && !phost->CbCmdWait(phost))
			return false;
	}
	eve_printf_debug("EVE register ID after wake up %x\n", (unsigned int)id);
	eve_assert(chipId == EVE_Hal_rd32(phost, REG_CHIP_ID));

	EVE_sleep(100);
	/* Read REG_CPURESET to check if engines are ready.
		Bit 0 for coprocessor engine,
		Bit 1 for touch engine,
		Bit 2 for audio engine.
	*/
	while ((engineStatus = EVE_Hal_rd8(phost, REG_CPURESET)) != 0x00)
	{
		if (engineStatus & 0x01)
		{
			eve_printf_debug("Coprocessor engine is not ready\n");
		}
		if (engineStatus & 0x02)
		{
			eve_printf_debug("Touch engine is not ready\n");
			if (EVE_HOST == EVE_HOST_BT8XXEMU)
			{
				EVE_Hal_wr8(phost, REG_CPURESET, 0);
			}
		}
		if (engineStatus & 0x04)
		{
			eve_printf_debug("Audio engine is not ready\n");
		}

		EVE_sleep(20);
		if (phost->CbCmdWait && !phost->CbCmdWait(phost))
			return false;
	}
	eve_printf_debug("All engines are ready\n");

	/* Update REG_FREQUENCY as per user selected */
	if (bootup->SystemClock != EVE_SYSCLK_DEFAULT)
	{
		uint32_t clockMHz = bootup->SystemClock * 12;
		EVE_Hal_wr32(phost, REG_FREQUENCY, clockMHz * 1000 * 1000);
		EVE_Hal_flush(phost);
		eve_printf_debug("EVE clock frequency set to %d MHz\n", (unsigned int)clockMHz);
	}
	else
	{
		eve_printf_debug("EVE default clock is %d MHz\n", (unsigned int)(EVE_Hal_rd32(phost, REG_FREQUENCY) / 1000000));
	}

	/* Switch to configured default SPI channel mode */
	EVE_Hal_setSPI(phost, bootup->SpiChannels);
#ifdef _DEBUG
	const char *spiChannels = (phost->SpiChannels == EVE_SPI_QUAD_CHANNEL)
								  ? "Quad"
								  : ((phost->SpiChannels == EVE_SPI_DUAL_CHANNEL) ? "Dual" : "Single");
	const char *requested = (bootup->SpiChannels == EVE_SPI_QUAD_CHANNEL)
								? "Quad"
								: ((bootup->SpiChannels == EVE_SPI_DUAL_CHANNEL) ? "Dual" : "Single");
	if (bootup->SpiChannels == phost->SpiChannels)
		eve_printf_debug("%s channel SPI\n", spiChannels);
	else
		eve_printf_debug("%s channel SPI (%s unsupported)\n", spiChannels, requested);
#endif

	EVE_Hal_wr32(phost, REG_GPIO_DIR, 0xffffffff);
	EVE_Hal_wr32(phost, REG_GPIO, 0xffffffff);

	return true;
}

/**
 * @brief Set up default parameters for bootup in EVE HAL framework
 *
 * @param phost Pointer to Hal context
 * @param config
 * @return true True if successful
 * @return false False if error
 */
bool EVE_Util_config(EVE_HalContext *phost, EVE_ConfigParameters *config)
{
	/* IMPORTANT: Do not use EVE_CoCmd functions here, as they can be overridden by hooks */

	bool reconfigure = phost->PCLK;
	if (reconfigure)
	{
		/* If PCLK is already set, we're reconfiguring. Quiet flush first */
		eve_printf_debug("Reconfiguring\n");
		EVE_Cmd_waitFlush(phost);
	}

	uint16_t wp;
	uint16_t rp;

	eve_assert(config->Width < 2048);
	eve_assert(config->Height < 2048);

	phost->PCLK = 0;

	EVE_Hal_wr16(phost, REG_HCYCLE, config->HCycle);
	EVE_Hal_wr16(phost, REG_HOFFSET, config->HOffset);
	EVE_Hal_wr16(phost, REG_HSYNC0, config->HSync0);
	EVE_Hal_wr16(phost, REG_HSYNC1, config->HSync1);
	EVE_Hal_wr16(phost, REG_VCYCLE, config->VCycle);
	EVE_Hal_wr16(phost, REG_VOFFSET, config->VOffset);
	EVE_Hal_wr16(phost, REG_VSYNC0, config->VSync0);
	EVE_Hal_wr16(phost, REG_VSYNC1, config->VSync1);
	EVE_Hal_wr8(phost, REG_PCLK_POL, config->PCLKPol);
	EVE_Hal_wr16(phost, REG_HSIZE, config->Width);
	EVE_Hal_wr16(phost, REG_VSIZE, config->Height);
	EVE_Hal_wr32(phost, REG_DISP, 1);
	EVE_Hal_wr32(phost, REG_PWM_DUTY, 128);

	phost->Width = config->Width;
	phost->Height = config->Height;

	// RAM size is a private address
	phost->DDR_RamSize = EVE_Hal_rd32(phost, REG_RAM_SIZE) * 1024 * 1024;
	eve_printf_debug("Ram size = %lu\n", phost->DDR_RamSize);

#if defined(DISPLAY_RESOLUTION_WSVGA)
	EVE_Hal_wr32(phost, REG_DISP, 1);
	EVE_Hal_wr32(phost, REG_PCLK_POL, 1);

	EVE_Hal_wr32(phost, REG_GPIO, 0x808b | (1 << 12));
	EVE_Hal_wr32(phost, REG_DISP, 1);
#endif

	EVE_Util_clearScreen(phost);

	/* Refresh fifo */
	wp = EVE_Cmd_wp(phost);
	rp = EVE_Cmd_rp(phost);
	EVE_Cmd_space(phost);
	phost->MediaFifoAddress = 0;
	phost->MediaFifoSize = 0;

	/* Coprocessor needs a reset */
	if (reconfigure ? EVE_CMD_FAULT(rp) : (wp || rp))
	{
		/* If this occurs after powerdown and bootup, powerdown is not working */
		eve_printf_debug("Coprocessor fifo not empty\n");
		if (!EVE_Util_resetCoprocessor(phost))
			return false;
	}
	else
	{
		/* Setup coprocessor defaults */
		eve_printf_debug("Configure coprocessor defaults\n");

		/* Reattach flash to avoid inconsistent state */
		EVE_Cmd_wr32(phost, CMD_FLASHDETACH);
		EVE_Cmd_wr32(phost, CMD_FLASHATTACH);

		phost->CoScratchHandle = 15;
	}

	EVE_Hal_flush(phost);
	if (!EVE_Cmd_waitFlush(phost))
		return false;

	eve_printf_debug("EVE configuration ready\n");
	return true;
}

/**
 * @brief Shut down the system
 *
 * @param phost Pointer to Hal context
 */
void EVE_Util_shutdown(EVE_HalContext *phost)
{
	EVE_Hal_wr32(phost, REG_GPIO_DIR, 0xffff);
	EVE_Hal_wr32(phost, REG_GPIO, 0);

	EVE_Hal_flush(phost);
	EVE_Hal_powerCycle(phost, false);
}

#if defined(_DEBUG)
/**
 * @brief Backup the last 128 bytes of RAM_G, which may be used for an error message
 *
 * @param phost Pointer to Hal context
 */
void debugBackupRamG(EVE_HalContext *phost)
{
	if (!phost->DebugMessageVisible)
	{
		EVE_Hal_rdMem(phost, phost->DebugBackup, RAM_G + phost->DDR_RamSize - sizeof(phost->DebugBackup), sizeof(phost->DebugBackup));
		phost->DebugMessageVisible = true;
	}
}
/**
 * @brief Restore the last 128 bytes of RAM_G
 *
 * @param phost Pointer to Hal context
 */
static void debugRestoreRamG(EVE_HalContext *phost)
{
	if (phost->DebugMessageVisible)
	{
		EVE_Hal_wrMem(phost, RAM_G + phost->DDR_RamSize - sizeof(phost->DebugBackup), phost->DebugBackup, sizeof(phost->DebugBackup));
		phost->DebugMessageVisible = false;
	}
}
#endif

/**
 * @brief Reset Coprocessor
 *
 * @param phost Pointer to Hal context
 * @return true True if successful
 * @return false False if error
 */
bool EVE_Util_resetCoprocessor(EVE_HalContext *phost)
{
	/* IMPORTANT: Do not use EVE_CoCmd functions here, as they can be overridden by hooks */
	bool ready;
	uint16_t rd;
	uint16_t wr;
	(void)rd;
	(void)wr;

	eve_printf_debug("Reset coprocessor\n");

	/* Set REG_CPURESET to 1, to hold the coprocessor in the reset condition */
	EVE_Hal_wr8(phost, REG_CPURESET, 1);
	EVE_Hal_flush(phost);
	EVE_sleep(100);

	/* Set REG_CMD_READ and REG_CMD_WRITE to zero */
	EVE_Hal_wr16(phost, REG_CMD_READ, 0);
	EVE_Hal_wr16(phost, REG_CMD_WRITE, 0);
	EVE_Hal_wr16(phost, REG_CMD_DL, 0);

	/* Stop playing audio in case video with audio was playing during reset */
	EVE_Hal_wr8(phost, REG_PLAYBACK_PLAY, 0);

	/* Default */
	phost->CmdFault = false;

	/* Set REG_CPURESET to 0, to restart the coprocessor */
	EVE_Hal_wr8(phost, REG_CPURESET, 0);
	EVE_Hal_flush(phost);
	EVE_sleep(100);
	eve_assert((wr = EVE_Hal_rd16(phost, REG_CMD_WRITE)) == 0);
	eve_assert((rd = EVE_Hal_rd16(phost, REG_CMD_READ)) == 0);

	/* Refresh fifo */
	EVE_Cmd_waitFlush(phost);
	phost->MediaFifoAddress = 0;
	phost->MediaFifoSize = 0;

	/* Cold start. Ensure that the coprocessor is ready. */
	EVE_Cmd_wr32(phost, CMD_DLSTART);
	EVE_Cmd_wr32(phost, CMD_COLDSTART);

	/* Reattach flash to avoid inconsistent state */
	EVE_Cmd_wr32(phost, CMD_FLASHATTACH);

	/* Wait for coprocessor to be ready */
	ready = EVE_Cmd_waitFlush(phost);
	if (phost->CbCoprocessorReset) /* Notify */
		phost->CbCoprocessorReset(phost, !ready);

#if defined(_DEBUG)
	debugRestoreRamG(phost);
#endif
	phost->CoScratchHandle = 15;
	return ready;
}

/**
 * @brief Bootup Coprocessor
 *
 * @param phost Pointer to Hal context
 * @return true True if successful
 * @return false False if error
 */
bool EVE_Util_bootupConfig(EVE_HalContext *phost)
{
	EVE_BootupParameters bootup;
	EVE_ConfigParameters config;

	EVE_Util_bootupDefaults(phost, &bootup);
	if (!EVE_Util_bootup(phost, &bootup))
	{
		return false;
	}

	EVE_Util_configDefaults(phost, &config, EVE_DISPLAY_DEFAULT);
	if (!EVE_Util_config(phost, &config))
	{
		EVE_Util_shutdown(phost);
		return false;
	}

	return true;
}

/**********************
** INTERACTIVE SETUP **
**********************/
/** @name INTERACTIVE SETUP */
///@{

/**
 * @brief
 *
 * @param phost Pointer to Hal context
 * @param flashFile
 * @return true True if successful
 * @return false False if error
 */
bool EVE_Util_openDeviceInteractive(EVE_HalContext *phost, const wchar_t *flashFile)
{
	EVE_CHIPID_T chipId;
	size_t deviceIdx;
	bool opened;
	EVE_HalParameters params = {0};

	/* Interactive device selection */
	EVE_Util_selectDeviceInteractive(&chipId, &deviceIdx);

	/* Fetch the default parameters for a device. Set the expected chip id.
	Pass the device index, or -1 to select the first device */
	EVE_Hal_defaultsEx(&params, deviceIdx);

	opened = EVE_Hal_open(phost, &params);

	if (!opened)
		return false;

	return true;
}

/**
 * @brief Calls EVE_Util_bootup and EVE_Util_config using the default parameters.
 * Falls back to no interactivity on FT9XX platform
 *
 * @param phost Pointer to Hal context
 * @param display
 * @return true True if successful
 * @return false False if error
 */
bool EVE_Util_bootupConfigInteractive(EVE_HalContext *phost, EVE_DISPLAY_T display)
{
	EVE_DISPLAY_T selectedDisplay;
	EVE_BootupParameters bootup;
	EVE_ConfigParameters config;

	EVE_Util_bootupDefaults(phost, &bootup);
	if (!EVE_Util_bootup(phost, &bootup))
	{
		return false;
	}

	EVE_Util_selectDisplayInteractive(&selectedDisplay);
	if (!selectedDisplay)
		selectedDisplay = display;

	EVE_Util_configDefaults(phost, &config, selectedDisplay);
	if (!EVE_Util_config(phost, &config))
	{
		EVE_Util_shutdown(phost);
		return false;
	}

	return true;
}

/**
 * @brief
 *
 * @param phost Pointer to Hal context
 * @param err
 */
void EVE_Util_forceFault(EVE_HalContext *phost, const char *err)
{
	/* Check if we're already in fault state */
	if (!EVE_Cmd_waitSpace(phost, 0))
		return;
#if 1
	/* Go into reset state and mimic a fault */
	EVE_Hal_wr8(phost, REG_CPURESET, 1);
	EVE_Hal_flush(phost);
	EVE_sleep(100);
	EVE_Hal_wr16(phost, REG_CMD_READ, 0xFFF);
	EVE_Hal_wr16(phost, REG_CMD_WRITE, 0xFFF);
#else
	/* TODO: Clean this up a bit */
	if (!EVE_Util_resetCoprocessor(phost))
		return;
	EVE_CoCmd_dlStart(phost);
	for (int i = 0; i < EVE_DL_COUNT; ++i)
		EVE_Cmd_wr32(phost, DISPLAY());
	EVE_Cmd_waitFlush(phost);
	EVE_Cmd_wr32(phost, DISPLAY());
	EVE_Hal_flush(phost);
	EVE_sleep(100);
#endif
	if (EVE_Cmd_rp(phost) & 0x3)
	{
		/* Write our own error to the error report area */
		uint32_t errLen = (uint32_t)strlen(err);
		EVE_Hal_wrMem(phost, RAM_ERR_REPORT, (const uint8_t *)err, errLen);
		if (errLen < RAM_ERR_REPORT_MAX)
		{
			EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, RAM_ERR_REPORT + errLen);
			for (int i = (int)errLen; i < RAM_ERR_REPORT_MAX; ++i)
				EVE_Hal_transfer8(phost, 0);
			EVE_Hal_endTransfer(phost);
		}
	}
	/* Fault state now */
	EVE_Cmd_waitFlush(phost);
#if 0
#if defined(_DEBUG)
	if (phost->DebugMessageVisible)
	{
		uint32_t errLen = (uint32_t)strlen(err);
		EVE_Hal_wrMem(phost, RAM_G + phost->DDR_RamSize - sizeof(phost->DebugBackup), (const uint8_t *)err, errLen);
		if (errLen < sizeof(phost->DebugBackup))
		{
			EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, RAM_G + phost->DDR_RamSize - sizeof(phost->DebugBackup) + errLen);
			for (int i = (int)errLen; i < sizeof(phost->DebugBackup); ++i)
				EVE_Hal_transfer8(phost, 0);
			EVE_Hal_endTransfer(phost);
		}
	}
#endif
#endif
}
///@}

/* end of file */
