/**
 * @file EW2024_Photobooth_Utils.c
 * @brief Utility functions for the EW2024 Photobooth application
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "EW2024_Photobooth_Utils.h"
#include <math.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char textbuffer[1000]; // for utils_drawTextFormat
static uint32_t a;
static uint32_t b;
static uint32_t c;
static uint32_t d;
static uint32_t e;
static uint32_t f;

uint8_t extsyncmode = 0;
int16_t w = 1920;
int16_t h = 1200;
uint16_t fbformat = YCBCR;
uint32_t framebuffer_addr;
uint32_t workarea;

/**
 * @brief Do calibration
 *
 * @return ft_bool_t True on successfull or otherwise
 */
bool utils_calibrateInit(EVE_HalContext *phost, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f)
{
	printf("utils_calibrateInit with data:0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n", a, b, c, d, e, f);

	if ((a | b | c | d | e | f) == 0)
	{
		printf("utils_calibrateInit zero\r\n");
		utils_calibrate(phost);
		utils_calibrateSave(phost);
		utils_calibrateDump(phost);
	}
	else
	{
		EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_A, a);
		EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_B, b);
		EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_C, c);
		EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_D, d);
		EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_E, e);
		EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_F, f);
		utils_calibrateSave(phost);
	}
	return 0;
}
bool utils_calibrate(EVE_HalContext *phost)
{
	uint32_t result;
	uint32_t transMatrix[6] = {0};
#if defined(EVE_SUPPORT_CAPACITIVE)
	EVE_Hal_wr8(phost, REG_CTOUCH_EXTENDED, CTOUCH_MODE_COMPATIBILITY);
#else
	EVE_Hal_wr8(phost, REG_TOUCH_ADC_MODE, ADC_DIFFERENTIAL);
#endif

	eve_printf_debug("App_CoPro_Widget_Calibrate: Start\n");

	EVE_CoCmd_watchdog(phost, 72000000); // Countermeasure for watchdog triggering when calibrate
	EVE_CoCmd_dlStart(phost);
	EVE_CoDl_clearColorRgb(phost, 64, 64, 64);
	EVE_CoDl_clear(phost, 1, 1, 1);
	EVE_CoDl_colorRgb(phost, 0xff, 0xff, 0xff);

	EVE_CoCmd_text(phost, (uint16_t)(phost->Width / 2), (uint16_t)(phost->Height / 2), 31, OPT_CENTER, "Please Tap on the dot");
	result = EVE_CoCmd_calibrate(phost, 0);
	EVE_Cmd_waitFlush(phost);

	eve_printf_debug("App_CoPro_Widget_Calibrate: End\n");

	// Print the configured values
	EVE_Hal_rdMem(phost, (uint8_t *)transMatrix, REG_TOUCH_TRANSFORM_A, 4 * 6); // read all the 6 coefficients
	eve_printf_debug("Touch screen transform values are A 0x%lx,B 0x%lx,C 0x%lx,D 0x%lx,E 0x%lx, F 0x%lx\n",
					 transMatrix[0], transMatrix[1], transMatrix[2], transMatrix[3], transMatrix[4], transMatrix[5]);

	return result != 0;
}

void utils_scale(int percent)
{
	EVE_CoCmd_loadIdentity(PHOST);
	EVE_CoCmd_scale(PHOST, percent * 65536L / 100, percent * 65536L / 100);
	EVE_CoCmd_setMatrix(PHOST);
}

/**
 * @brief Callback function
 *
 * @param phost
 * @return true
 */
bool cbCmdWait(struct EVE_HalContext *phost)
{
	(void)(phost); // suspress warning on unused
	return true;
}

/**
 * @brief EVE initialzation
 *
 * @param phost
 */
void utils_gpuInit(EVE_HalContext *phost)
{
	size_t deviceIdx;

	EVE_HalParameters params;

#if defined(BT8XXEMU_PLATFORM)
	printf(BT8XXEMU_version());
	BT8XXEMU_EmulatorParameters emulatorParams;
	BT8XXEMU_FlashParameters flashParams;
#endif

	EVE_Hal_initialize();

	deviceIdx = -1;

	EVE_Hal_defaultsEx(&params, deviceIdx);
	params.CbCmdWait = cbCmdWait;

	EVE_Hal_open(phost, &params);

	EVE_Util_bootupConfig(phost);

#ifdef DISPLAY_RESOLUTION_WXGA
	EVE_Hal_wr8(phost, REG_CPURESET, 2);
	EVE_Hal_wr16(phost, REG_TOUCH_CONFIG, 0x05D0); // for Goodix touch panel
	EVE_Hal_wr8(phost, REG_CPURESET, 0);
	EVE_sleep(300);
#endif

#if defined(FT900_PLATFORM) || defined(FT93X_PLATFORM) || defined(RP2040_PLATFORM)
	EVE_Util_loadSdCard(phost);
#endif
}

void utils_gpuRelease(EVE_HalContext *phost)
{
	EVE_Hal_close(phost);
	EVE_Hal_release();
}

/**
 * @brief Get system frequency in Hz
 *
 * @param phost EVE_HalContext* pointer
 * @return uint32_t System clock frequency in Hz
 */
uint32_t utils_getSystemClock(EVE_HalContext *phost)
{
	EVE_Hal_rd32(phost, REG_CLOCK); // warm up

	uint32_t c0 = EVE_Hal_rd32(phost, REG_CLOCK);
	uint32_t c1 = EVE_Hal_rd32(phost, REG_CLOCK);

	uint32_t overhead = c1 - c0;

	c0 = EVE_Hal_rd32(phost, REG_CLOCK);
	EVE_sleep(1000);
	c1 = EVE_Hal_rd32(phost, REG_CLOCK);

	uint32_t freq = (uint32_t)round((c1 - c0) - overhead);

	return freq;
}

void utils_loadImageToRamg(EVE_HalContext *phost, const char *fileName, int32_t destination, uint8_t type)
{
	if (type == LOADIMAGE)
	{
		EVE_Util_loadImageFile(phost, destination, fileName, NULL);
	}
	else if (type == INFLATE)
	{
		EVE_Util_loadInflateFile(phost, destination, fileName);
	}
	else if (type == LOAD)
	{
		EVE_Util_loadRawFile(phost, destination, fileName);
	}
	else
	{
	}
}

void utils_calibrateRestore(EVE_HalContext *phost)
{
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_A, a);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_B, b);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_C, c);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_D, d);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_E, e);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_F, f);
}

void utils_calibrateSave(EVE_HalContext *phost)
{
	a = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_A);
	b = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_B);
	c = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_C);
	d = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_D);
	e = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_E);
	f = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_F);
}

void utils_calibrateDump(EVE_HalContext *phost)
{
	printf("utils_calibrateInit(PHOST, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x);\r\n", a, b, c, d, e, f);
}

void utils_displayStartColor(EVE_HalContext *phost, uint8_t *bgColor, uint8_t *textColor)
{
	EVE_CoCmd_dlStart(phost);
	EVE_CoDl_clearColorRgb(phost, bgColor[0], bgColor[1], bgColor[2]);
	EVE_CoDl_clear(phost, 1, 1, 1);
	EVE_CoDl_colorRgb(phost, textColor[0], textColor[1], textColor[2]);
	vertex_format(PHOST, 1);
}

void utils_displayStart(EVE_HalContext *phost)
{
	utils_displayStartColor(phost, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
}

void utils_displayEnd(EVE_HalContext *phost)
{
	EVE_CoDl_display(PHOST);
	EVE_CoCmd_swap(PHOST);
	EVE_CoCmd_graphicsfinish(PHOST);
	EVE_Cmd_waitFlush(PHOST);
}

void utils_playMuteSound(EVE_HalContext *phost)
{
	EVE_Hal_wr16(phost, REG_SOUND, 0x0060);
	EVE_Hal_wr8(phost, REG_PLAY, 0x01);
}

void utils_playSound(EVE_HalContext *phost, uint8_t sound, uint8_t vol, uint8_t midi)
{
	uint16_t val = (uint16_t)((midi << 8) | sound);
	EVE_Hal_wr8(phost, REG_VOL_SOUND, vol);
	EVE_Hal_wr16(phost, REG_SOUND, val);
	EVE_Hal_wr8(phost, REG_PLAY, 1);
}

uint8_t utils_isSDcardDetected()
{
#if defined(MSVC_PLATFORM) || defined(BT8XXEMU_PLATFORM)
	return true;
#elif defined(FT9XX_PLATFORM)
	return sdhost_card_detect() == SDHOST_CARD_INSERTED;
#else
	return true;
#endif
}

/**
 * @brief picture ans testcard
 *
 */
void utils_lvdsConfig(EVE_HalContext *phost, uint16_t format, uint8_t testcase)
{
	uint8_t TXPLLDiv = 0;

	fbformat = format;
#if defined(DISPLAY_RESOLUTION_WSVGA)
	extsyncmode = 1; // 0: 1 pixel single // 1: 2 pixel single // 2: 2 pixel dual // 3: 4 pixel dual
	w = 1024;
	h = 600;
	EVE_Hal_wr32(phost, LVDSPLL_CFG, 0x00301075);
#elif defined(DISPLAY_RESOLUTION_WUXGA)
	w = 1920;
	h = 1200;
	extsyncmode = 3; // 0: 1 pixel single // 1: 2 pixel single // 2: 2 pixel dual // 3: 4 pixel dual
	TXPLLDiv = 0x03;
	eve_printf_debug("TXPLLDiv %d\n", TXPLLDiv);
	EVE_Hal_wr32(phost, REG_LVDS_PLLCFG, (TXPLLDiv > 4 ? (0x00300870 + TXPLLDiv) : (0x00301070 + TXPLLDiv)));
#endif
	EVE_Hal_wr32(phost, REG_LVDS_EN, 0);
	EVE_Hal_wr32(phost, REG_LVDS_EN, 7);
	EVE_sleep(10);
	eve_printf_debug("LVDS_EN: %lx \n", EVE_Hal_rd32(phost, REG_LVDS_EN));
	eve_printf_debug("LVDSPLL_CFG: %lx \n", EVE_Hal_rd32(phost, REG_LVDS_PLLCFG));
	eve_printf_debug("REG_LVDS_CTRL_CH0: %lx \n", EVE_Hal_rd32(phost, REG_LVDS_CTRL_CH0));
	eve_printf_debug("REG_LVDS_CTRL_CH1: %lx \n", EVE_Hal_rd32(phost, REG_LVDS_CTRL_CH1));

	EVE_Hal_wr32(phost, REG_I2S_CTL, 0x2);
	EVE_Hal_wr32(phost, REG_I2S_CFG, 0x400);
	EVE_Hal_wr32(phost, REG_I2S_EN, 1);
	EVE_Hal_wr32(phost, REG_I2S_FREQ, 0x3CF0);

	EVE_Hal_wr32(phost, REG_SC0_SIZE, 2);
	EVE_Hal_wr32(phost, REG_SC0_PTR0, SC0_PTR0_STARTADDR);
	EVE_Hal_wr32(phost, REG_SC0_PTR1, SC0_PTR1_STARTADDR);
#if 1
	EVE_Hal_wr32(phost, REG_SC1_SIZE, 2);
	EVE_Hal_wr32(phost, REG_SC1_PTR0, SC1_PTR0_STARTADDR);
	EVE_Hal_wr32(phost, REG_SC1_PTR1, SC1_PTR1_STARTADDR);

	EVE_Hal_wr32(phost, REG_SC2_SIZE, 2);
	EVE_Hal_wr32(phost, REG_SC2_PTR0, SC2_PTR0_STARTADDR);
	EVE_Hal_wr32(phost, REG_SC2_PTR1, SC2_PTR1_STARTADDR);
#endif
	framebuffer_addr = DDR_FRAMEBUFFER_STARTADDR;
	workarea = DDR_WORKAREA;
	EVE_Cmd_waitFlush(phost);

	if (testcase == TESTCASE_PICTURE)
	{
		utils_lvdsScanoutSingle(phost);
	}
	else if (testcase == TESTCASE_TESTCARD)
	{
		utils_lvdsScanoutSingle(phost);

		EVE_CoCmd_testCard(phost);
		EVE_Cmd_waitFlush(phost);
		EVE_CoCmd_dlStart(phost);
	}
	else if (testcase == TESTCASE_SWAPPING)
	{
		utils_lvdsScanoutSwaping(phost);

		EVE_Cmd_wr32(phost, CLEAR_COLOR_RGB(0, 255, 0));
		EVE_Cmd_wr32(phost, CLEAR(1, 1, 1));
		EVE_Cmd_wr32(phost, DISPLAY());
		EVE_CoCmd_swap(phost);
		EVE_Cmd_waitFlush(phost);

		EVE_CoCmd_dlStart(phost);
	}
	else if (testcase == TESTCASE_MOCK_LVDSRX)
	{
	}

	else if (testcase == TESTCASE_DIRECTVIDEO)
	{
		utils_lvdsScanoutSwaping(phost);

		EVE_Hal_wr32(phost, REG_SO_EN, 0);
		EVE_Hal_wr32(phost, REG_SC1_RESET, 1);
		EVE_Hal_wr32(phost, REG_SO_SOURCE, SWAPCHAIN_1);
		EVE_Hal_wr32(phost, REG_SO_EN, 1);
	}
	else if (testcase == TESTCASE_VIDEO)
	{
		utils_lvdsScanoutSwaping(phost);
	}
	else if (testcase == TESTCASE_TOUCH)
	{
		utils_lvdsScanoutSwaping(phost);
	}
	else if (testcase == TESTCASE_PLAYBACK)
	{
		utils_lvdsScanoutSwaping(phost);
	}

	EVE_Cmd_waitFlush(phost);
	eve_printf_debug("REG_LVDS_STAT is %lx \n", EVE_Hal_rd32(phost, REG_LVDS_STAT));
	eve_printf_debug("REG_LVDS_ERR_STAT is %lx \n", EVE_Hal_rd32(phost, REG_LVDS_ERR_STAT));
}

void utils_lvdsScanoutSwaping(EVE_HalContext *phost)
{
	EVE_Hal_wr32(phost, REG_SO_MODE, extsyncmode);
	EVE_Hal_wr32(phost, REG_SO_SOURCE, SWAPCHAIN_0);
	EVE_Hal_wr32(phost, REG_SO_FORMAT, fbformat);
	EVE_Hal_wr32(phost, REG_SO_EN, 1);
	EVE_CoCmd_renderTarget(phost, SWAPCHAIN_0, fbformat, w, h);
}

void utils_lvdsScanoutSingle(EVE_HalContext *phost)
{
	EVE_Hal_wr32(phost, REG_SO_MODE, extsyncmode);
	EVE_Hal_wr32(phost, REG_SO_SOURCE, framebuffer_addr);
	EVE_Hal_wr32(phost, REG_SO_FORMAT, fbformat);
	EVE_Hal_wr32(phost, REG_SO_EN, 1);
	EVE_CoCmd_renderTarget(phost, framebuffer_addr, fbformat, w, h);
}

format_info_t format_info_table[] = {
	{.format_value = L1, .format_name = "L1", .bits_per_pixel = 1},
	{.format_value = L4, .format_name = "L4", .bits_per_pixel = 4},
	{.format_value = L8, .format_name = "L8", .bits_per_pixel = 8},
	{.format_value = RGB332, .format_name = "RGB332", .bits_per_pixel = 8},
	{.format_value = ARGB2, .format_name = "ARGB2", .bits_per_pixel = 8},
	{.format_value = ARGB4, .format_name = "ARGB4", .bits_per_pixel = 16},
	{.format_value = RGB565, .format_name = "RGB565", .bits_per_pixel = 16},
	{.format_value = BARGRAPH, .format_name = "BARGRAPH", .bits_per_pixel = 8},
	{.format_value = L2, .format_name = "L2", .bits_per_pixel = 2},
	{.format_value = RGB8, .format_name = "RGB8", .bits_per_pixel = 24},
	{.format_value = ARGB8, .format_name = "ARGB8", .bits_per_pixel = 32},
	{.format_value = PALETTEDARGB8, .format_name = "PALETTEDARGB8", .bits_per_pixel = 8},
	{.format_value = RGB6, .format_name = "RGB6", .bits_per_pixel = 18},
	{.format_value = ARGB6, .format_name = "ARGB6", .bits_per_pixel = 24},
	{.format_value = LA1, .format_name = "LA1", .bits_per_pixel = 2},
	{.format_value = LA2, .format_name = "LA2", .bits_per_pixel = 4},
	{.format_value = LA4, .format_name = "LA4", .bits_per_pixel = 8},
	{.format_value = LA8, .format_name = "LA8", .bits_per_pixel = 16},
	{.format_value = YCBCR, .format_name = "YCBCR", .bits_per_pixel = 8},
	{.format_value = COMPRESSED_RGBA_ASTC_4x4_KHR, .format_name = "COMPRESSED_RGBA_ASTC_4x4_KHR", .bits_per_pixel = 800},
	{.format_value = COMPRESSED_RGBA_ASTC_5x4_KHR, .format_name = "COMPRESSED_RGBA_ASTC_5x4_KHR", .bits_per_pixel = 640},
	{.format_value = COMPRESSED_RGBA_ASTC_5x5_KHR, .format_name = "COMPRESSED_RGBA_ASTC_5x5_KHR", .bits_per_pixel = 512},
	{.format_value = COMPRESSED_RGBA_ASTC_6x5_KHR, .format_name = "COMPRESSED_RGBA_ASTC_6x5_KHR", .bits_per_pixel = 427},
	{.format_value = COMPRESSED_RGBA_ASTC_6x6_KHR, .format_name = "COMPRESSED_RGBA_ASTC_6x6_KHR", .bits_per_pixel = 356},
	{.format_value = COMPRESSED_RGBA_ASTC_8x5_KHR, .format_name = "COMPRESSED_RGBA_ASTC_8x5_KHR", .bits_per_pixel = 320},
	{.format_value = COMPRESSED_RGBA_ASTC_8x6_KHR, .format_name = "COMPRESSED_RGBA_ASTC_8x6_KHR", .bits_per_pixel = 267},
	{.format_value = COMPRESSED_RGBA_ASTC_8x8_KHR, .format_name = "COMPRESSED_RGBA_ASTC_8x8_KHR", .bits_per_pixel = 200},
	{.format_value = COMPRESSED_RGBA_ASTC_10x5_KHR, .format_name = "COMPRESSED_RGBA_ASTC_10x5_KHR", .bits_per_pixel = 256},
	{.format_value = COMPRESSED_RGBA_ASTC_10x6_KHR, .format_name = "COMPRESSED_RGBA_ASTC_10x6_KHR", .bits_per_pixel = 213},
	{.format_value = COMPRESSED_RGBA_ASTC_10x8_KHR, .format_name = "COMPRESSED_RGBA_ASTC_10x8_KHR", .bits_per_pixel = 160},
	{.format_value = COMPRESSED_RGBA_ASTC_10x10_KHR, .format_name = "COMPRESSED_RGBA_ASTC_10x10_KHR", .bits_per_pixel = 128},
	{.format_value = COMPRESSED_RGBA_ASTC_12x10_KHR, .format_name = "COMPRESSED_RGBA_ASTC_12x10_KHR", .bits_per_pixel = 107},
	{.format_value = COMPRESSED_RGBA_ASTC_12x12_KHR, .format_name = "COMPRESSED_RGBA_ASTC_12x12_KHR", .bits_per_pixel = 89},
};

format_info_t *utils_searchFormatInfo(uint32_t format_value)
{
	for (int i = 0; i < sizeof(format_info_table) / sizeof(format_info_table[0]); i++)
	{
		if (format_info_table[i].format_value == format_value)
		{
			return &format_info_table[i];
		}
	}
	return NULL;
}
uint32_t utils_searchBitsPerPixel(uint32_t bitmap_fmt)
{
	format_info_t *pFormatInfo = utils_searchFormatInfo(bitmap_fmt);
	if (pFormatInfo)
	{
		return pFormatInfo->bits_per_pixel;
	}
	return 1;
}

void utils_drawBox(box *b)
{
	return utils_drawRect(b->x, b->y, b->w, b->h);
}
void utils_drawBoxAt(box *b, int32_t x, int32_t y)
{
	return utils_drawRect(x, y, b->w, b->h);
}
void utils_drawRect(int32_t x, int32_t y, int32_t w, int32_t h)
{
	begin(PHOST, RECTS);
	vertex2f(PHOST, x, y);
	vertex2f(PHOST, x + w, y + h);
}

void (*vertex2f_rewrite)(EVE_HalContext *phost, int32_t x, int32_t y) = vertex2f_16;
void vertex2f_1(EVE_HalContext *phost, int32_t x, int32_t y) { EVE_Cmd_wr32(phost, VERTEX2F((x), (y))); }
void vertex2f_2(EVE_HalContext *phost, int32_t x, int32_t y) { EVE_Cmd_wr32(phost, VERTEX2F((x) * 2, (y) * 2)); }
void vertex2f_4(EVE_HalContext *phost, int32_t x, int32_t y) { EVE_Cmd_wr32(phost, VERTEX2F((x) * 4, (y) * 4)); }
void vertex2f_8(EVE_HalContext *phost, int32_t x, int32_t y) { EVE_Cmd_wr32(phost, VERTEX2F((x) * 8, (y) * 8)); }
void vertex2f_16(EVE_HalContext *phost, int32_t x, int32_t y) { EVE_Cmd_wr32(phost, VERTEX2F((x) * 16, (y) * 16)); }

void pointsize_rewrite(EVE_HalContext *phost, uint32_t size)
{
	EVE_Cmd_wr32(phost, POINT_SIZE(size * 16));
}
void vertex_format_auto(EVE_HalContext *phost)
{
/// Frac value Unit  range
/// 0    1 𝑝𝑖𝑥𝑒𝑙      -16384 to 16383
/// 1    ½ 𝑝𝑖𝑥𝑒𝑙      -8192 to 8191
/// 2    ¼ 𝑝𝑖𝑥𝑒𝑙      -4096 to 4095
/// 3    1/8 𝑝𝑖𝑥𝑒𝑙    -2048 to 2047
/// 4    1/16 𝑝𝑖𝑥𝑒𝑙   -1024 to 1023
/// Table 35 – VERTEX_FORMAT and Pixel Precision
#define FRAC0 16384
#define FRAC1 (FRAC0 / 2)
#define FRAC2 (FRAC1 / 2)
#define FRAC3 (FRAC2 / 2)
#define FRAC4 (FRAC3 / 2)
	uint32_t format = 0;

	if (phost->Width <= FRAC4)
		format = 4;
	else if (phost->Width <= FRAC3)
		format = 3;
	else if (phost->Width <= FRAC2)
		format = 2;
	else if (phost->Width <= FRAC1)
		format = 1;
	else if (phost->Width <= FRAC0)
		format = 0;

	vertex_format_rewrite(phost, format);
}

void vertex_format_rewrite(EVE_HalContext *phost, uint32_t format)
{
	void (*vertex[])(uint32_t x, uint32_t y) = {
		vertex2f_1,
		vertex2f_2,
		vertex2f_4,
		vertex2f_8,
		vertex2f_16};

	int numOfElements = sizeof(vertex) / sizeof(vertex[0]);
	if (format >= 0 && format < numOfElements)
	{
		vertex2f_rewrite = vertex[format];
		cmd(phost, VERTEX_FORMAT(format));
	}
}
