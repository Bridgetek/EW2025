/**
 * @file Draw.c
 * @brief Utility functions for drawing tags and other shapes on the EVE display
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "EW2024_Photobooth_Utils.h"
#include "Draw.h"

void utils_colorTag(EVE_HalContext *phost, uint8_t *color, uint32_t tag)
{
	if (tag)
		Tag(phost, tag);
	if (color)
	{
		ColorRGB(phost, color[0], color[1], color[2]);
	}
}

uint32_t utils_drawBoxTextLeft(EVE_HalContext *phost, uint32_t x, uint32_t y, uint32_t w, uint32_t h,
							   uint32_t rounded, uint8_t *color, uint32_t tag, char *text, uint32_t font, uint32_t otp, uint8_t *tcolor)
{
	if (w < 1 || h < 1)
		return 0;

	utils_colorTag(phost, color, tag);
	Begin(phost, RECTS);
	Vertex2f(phost, x + rounded / 2, y + rounded / 2);
	Vertex2f(phost, x + w - rounded / 2, y + h - rounded / 2);

	if (text && strnlen(text, 100) > 0)
	{
		uint32_t tx = x + 5;
		uint32_t ty = y + h / 2;
		uint32_t totp = OPT_CENTERY;
		if (tcolor)
		{
			ColorRGB(phost, tcolor[0], tcolor[1], tcolor[2]);
		}

		cmd_text(phost, tx, ty, font, otp | totp, text);
	}
	if (tcolor || color)
	{
		ColorRGB(phost, 255, 255, 255);
	}
	return 1;
}

uint32_t utils_drawBoxText(EVE_HalContext *phost, uint32_t x, uint32_t y, uint32_t w, uint32_t h,
						   uint32_t rounded, uint8_t *color, uint32_t tag, char *text, uint32_t font, uint32_t otp, uint8_t *tcolor)
{
	if (w < 1 || h < 1)
		return 0;

	utils_colorTag(phost, color, tag);
	Begin(phost, RECTS);
	Vertex2f(phost, x + rounded / 2, y + rounded / 2);
	Vertex2f(phost, x + w - rounded / 2, y + h - rounded / 2);

	if (text && strnlen(text, 100) > 0)
	{
		uint32_t tx = x + w / 2;
		uint32_t ty = y + h / 2;
		uint32_t totp = OPT_CENTER;
		if (tcolor)
		{
			ColorRGB(phost, tcolor[0], tcolor[1], tcolor[2]);
		}

		cmd_text(phost, tx, ty, font, otp | totp, text);
	}
	if (tcolor || color)
	{
		ColorRGB(phost, 255, 255, 255);
	}
	return 1;
}
/**
 * @brief Helper function to draw a point
 *
 * @param phost
 * @param Rx
 * @param Ry
 * @param R
 * @return uint32_t
 */
uint32_t utils_drawPoint(EVE_HalContext *phost, uint32_t Rx, uint32_t Ry, uint32_t R)
{
	EVE_CoDl_pointSize(phost, R * 16);
	EVE_CoDl_begin(phost, POINTS);
	vertex2f(phost, Rx, Ry);
	return 1;
}

static void utils_gpuText(EVE_HalContext *phost, const char *str, uint8_t *bgColor, uint8_t *textColor)
{
	uint16_t font = 0;

	if (phost->Width == 480)
	{
		font = 28;
	}
	else if (phost->Width == 800)
	{
		font = 30;
	}
	else if (phost->Width == 1024)
	{
		font = 31;
	}
	else if (phost->Width == 1920)
	{
		font = 31;
	}
	else
	{
		font = 27;
	}

	utils_displayStartColor(phost, bgColor, textColor);
	EVE_CoCmd_fillWidth(phost, phost->Width);
	EVE_CoCmd_text(phost, (uint16_t)(phost->Width / 2),
				   (uint16_t)(phost->Height / 2), font, OPT_CENTERX | OPT_FILL, str);
	utils_displayEnd(phost);
}

void utils_drawTextColor(EVE_HalContext *phost, const char *str, uint8_t *bgColor, uint8_t *textColor)
{
	utils_gpuText(phost, str, bgColor, textColor);
	EVE_sleep(2000);
}

void utils_drawText(EVE_HalContext *phost, const char *str)
{
	utils_gpuText(phost, str, (uint8_t[]){0xff, 0xff, 0xff}, (uint8_t[]){0, 0, 0});
	EVE_sleep(2000);
}

void utils_drawText2(EVE_HalContext *phost, const char *str, int ms)
{
	utils_gpuText(phost, str, (uint8_t[]){0xff, 0xff, 0xff}, (uint8_t[]){0, 0, 0});
	EVE_sleep(ms);
}

void utils_hintScreen(EVE_HalContext *phost, const char *msg)
{
	utils_drawText(phost, msg);
}

void utils_showDialogInfo(EVE_HalContext *phost, const uint8_t *msg)
{
	const uint16_t border = 2; // pixel
	const uint16_t base_w = 800;
	const uint16_t base_h = 600;
	uint16_t w = 400;
	uint16_t h = 200;
	uint16_t x = (base_w - w) / 2;
	uint16_t y = (base_h - h) / 2;
	uint8_t htop = 50;
	uint8_t hbottom = 50;
	uint8_t font = 20;

	uint16_t btn_h = 40;
	uint16_t btn_w = 100;

	uint32_t ratio = base_w * 100 / phost->Width;

	if (ratio != 100)
	{
		w = w * 100 / ratio;
		h = h * 100 / ratio;
		btn_w = btn_w * 100 / ratio;
		btn_h = btn_h * 100 / ratio;
		x = (phost->Width - w) / 2;
		y = (phost->Height - h) / 2;
		htop = htop * 100 / ratio;
		hbottom = hbottom * 100 / ratio;

		ratio += (100 - ratio) / 2;
		font = font * 100 / ratio;
		if (font > 31)
			font = 31;
		if (font < 16)
			font = 16;
		if (font == 17 || font == 19)
			font = 18;
	}

	utils_displayStart(phost);

	/// fade the whole LCD
	EVE_Cmd_wr32(phost, COLOR_RGB(0x3F, 0x3F, 0x3F));
	EVE_Cmd_wr32(phost, BEGIN(RECTS));
	EVE_Cmd_wr32(phost, VERTEX2F(0, 0));
	vertex2f(phost, phost->Width, phost->Height);

	/// diag border
	EVE_Cmd_wr32(phost, COLOR_RGB(0xE1, 0xE1, 0xE1));
	EVE_Cmd_wr32(phost, BEGIN(RECTS));
	vertex2f(phost, x, y);
	vertex2f(phost, x + w, y + h);

	/// center part
	EVE_Cmd_wr32(phost, COLOR_RGB(0xFF, 0xFF, 0xFF));
	EVE_Cmd_wr32(phost, BEGIN(RECTS));
	vertex2f(phost, x + border, y + border);
	vertex2f(phost, x + w - border, y + h - border);

	/// top part
	EVE_Cmd_wr32(phost, COLOR_RGB(0xF3, 0xF3, 0xF3));
	EVE_Cmd_wr32(phost, BEGIN(RECTS));
	vertex2f(phost, x + border, y + border);
	vertex2f(phost, x + w - border, y + htop);

	/// bottom part
	EVE_Cmd_wr32(phost, COLOR_RGB(0xF3, 0xF3, 0xF3));
	EVE_Cmd_wr32(phost, BEGIN(RECTS));
	vertex2f(phost, x + border, y + h - hbottom + border);
	vertex2f(phost, x + w - border, y + h - border);

	font = font * 8 / 10;
	if (font < 16)
		font = 16;
	if (font == 17 || font == 19)
		font = 18;
	EVE_Cmd_wr32(phost, COLOR_RGB(0x78, 0x78, 0x78));
	EVE_CoCmd_text(phost, x + border + 30, y + h / 2 - 20, font, 0, (const char *)msg);

	utils_displayEnd(phost);
}

uint8_t utils_showDialogYesno(EVE_HalContext *phost, const char *title, const char *msg)
{
	const uint32_t tag_y = 1;
	const uint32_t tag_n = 2;

	const uint16_t border = 2; // pixel
	const uint16_t base_w = 800;
	const uint16_t base_h = 600;
	uint16_t w = 400;
	uint16_t h = 200;
	uint16_t x = (base_w - w) / 2;
	uint16_t y = (base_h - h) / 2;
	uint8_t htop = 50;
	uint8_t hbottom = 50;
	uint8_t font = 20;

	uint16_t btn_h = 30;
	uint16_t btn_w = 80;

	uint32_t ratio = base_w * 100 / phost->Width;

	if (ratio != 100)
	{
		w = w * 100 / ratio;
		h = h * 100 / ratio;
		btn_w = btn_w * 100 / ratio;
		btn_h = btn_h * 100 / ratio;
		x = (phost->Width - w) / 2;
		y = (phost->Height - h) / 2;
		htop = htop * 100 / ratio;
		hbottom = hbottom * 100 / ratio;

		ratio += (100 - ratio) / 2;
		font = font * 100 / ratio;
		if (font > 31)
			font = 31;
		if (font < 16)
			font = 16;
		if (font == 17 || font == 19)
			font = 18;
	}

	do
	{
		utils_displayStart(phost);

		/// fade the whole LCD
		EVE_Cmd_wr32(phost, COLOR_RGB(0x3F, 0x3F, 0x3F));
		EVE_Cmd_wr32(phost, BEGIN(RECTS));
		vertex2f(phost, 0, 0);
		vertex2f(phost, phost->Width, phost->Height);

		/// diag border
		EVE_Cmd_wr32(phost, COLOR_RGB(0xE1, 0xE1, 0xE1));
		EVE_Cmd_wr32(phost, BEGIN(RECTS));
		vertex2f(phost, x, y);
		vertex2f(phost, x + w, y + h);

		/// center part
		EVE_Cmd_wr32(phost, COLOR_RGB(0xFF, 0xFF, 0xFF));
		EVE_Cmd_wr32(phost, BEGIN(RECTS));
		vertex2f(phost, x + border, y + border);
		vertex2f(phost, x + w - border, y + h - border);

		/// top part
		EVE_Cmd_wr32(phost, COLOR_RGB(0x46, 0x50, 0x52));
		EVE_Cmd_wr32(phost, BEGIN(RECTS));
		vertex2f(phost, x + border, y + border);
		vertex2f(phost, x + w - border, y + htop);

		/// bottom part
		EVE_Cmd_wr32(phost, COLOR_RGB(0xF3, 0xF3, 0xF3));
		EVE_Cmd_wr32(phost, BEGIN(RECTS));
		vertex2f(phost, x + border, y + h - hbottom + border);
		vertex2f(phost, x + w - border, y + h - border);

		/// diag message
		EVE_Cmd_wr32(phost, COLOR_RGB(0xFF, 0xFF, 0xFF));
		EVE_CoCmd_text(phost, x + border + 10, y + border + 10, font, 0, title);

		uint32_t font2 = font * 8 / 10;
		if (font2 < 16)
			font2 = 16;
		if (font2 == 17 || font2 == 19)
			font2 = 18;
		EVE_Cmd_wr32(phost, COLOR_RGB(0x78, 0x78, 0x78));
		EVE_CoCmd_fillWidth(phost, w);
		EVE_CoCmd_text(phost, x + border + 30, y + h / 2 - 20, font2, OPT_FILL, msg);

		/// diag button yes/no
		EVE_Cmd_wr32(phost, COLOR_RGB(0xFF, 0xFF, 0xFF));
		const uint16_t btn_margin = 25;
		EVE_Cmd_wr32(phost, TAG(tag_y));
		EVE_CoCmd_button(phost, x + w / 2 - btn_w - btn_margin,
						 y + h - hbottom + border + (hbottom - btn_h) / 2, btn_w, btn_h,
						 font, 0, "Yes");

		EVE_Cmd_wr32(phost, TAG(tag_n));
		EVE_CoCmd_button(phost, x + w / 2 + btn_margin,
						 y + h - hbottom + border + (hbottom - btn_h) / 2, btn_w, btn_h,
						 font, 0, "No");

		utils_displayEnd(phost);

		utils_gestureRenew(phost);
		if (utils_gestureGet()->tagReleased == tag_y)
		{
			return true;
		}
		else if (utils_gestureGet()->tagReleased == tag_n)
		{
			return false;
		}
	} while (true);

	return false;
}

/* This image contains cells: 32x32
Cell 0: Home
Cell 1: Star
Cell 2: Down triangle
Cell 3: Up triangle
Cell 4: Right triangle (Play button icon)
Size: 460 bytes */
PROGMEM const unsigned char home_start_icon[] = {0x78, 0x9C, 0xE5, 0x94, 0xBF, 0x4E, 0xC2, 0x40, 0x1C, 0xC7, 0x7F, 0x2D, 0x04, 0x8B, 0x20, 0x45, 0x76, 0x14, 0x67, 0xA3, 0xF1, 0x0D, 0x64, 0x75, 0xD2, 0xD5, 0x09, 0x27, 0x17, 0x13, 0xE1, 0x0D, 0xE4, 0x0D, 0x78, 0x04, 0x98, 0x5D, 0x30, 0x26, 0x0E, 0x4A, 0xA2, 0x3E, 0x82, 0x0E, 0x8E, 0x82, 0xC1, 0x38, 0x62, 0x51, 0x0C, 0x0A, 0x42, 0x7F, 0xDE, 0xB5, 0x77, 0xB4, 0x77, 0x17, 0x28, 0x21, 0x26, 0x46, 0xFD, 0x26, 0xCD, 0xE5, 0xD3, 0x7C, 0xFB, 0xBB, 0xFB, 0xFD, 0xB9, 0x02, 0xCC, 0xA4, 0xE8, 0x99, 0x80, 0x61, 0xC4, 0x8A, 0x9F, 0xCB, 0x6F, 0x31, 0x3B, 0xE3, 0x61, 0x7A, 0x98, 0x84, 0x7C, 0x37, 0xF6, 0xFC, 0xC8, 0xDD, 0x45, 0x00, 0xDD, 0xBA, 0xC4, 0x77, 0xE6, 0xEE, 0x40, 0xEC, 0x0E, 0xE6, 0x91, 0xF1, 0xD2, 0x00, 0x42, 0x34, 0x5E, 0xCE, 0xE5, 0x08, 0x16, 0xA0, 0x84, 0x68, 0x67, 0xB4, 0x86, 0xC3, 0xD5, 0x26, 0x2C, 0x20, 0x51, 0x17, 0xA2, 0xB8, 0x03, 0xB0, 0xFE, 0x49, 0xDD, 0x54, 0x15, 0xD8, 0xEE, 0x73, 0x37, 0x95, 0x9D, 0xD4, 0x1A, 0xB7, 0xA5, 0x26, 0xC4, 0x91, 0xA9, 0x0B, 0x06, 0xEE, 0x72, 0xB7, 0xFB, 0xC5, 0x16, 0x80, 0xE9, 0xF1, 0x07, 0x8D, 0x3F, 0x15, 0x5F, 0x1C, 0x0B, 0xFC, 0x0A, 0x90, 0xF0, 0xF3, 0x09, 0xA9, 0x90, 0xC4, 0xC6, 0x37, 0xB0, 0x93, 0xBF, 0xE1, 0x71, 0xDB, 0xA9, 0xD7, 0x41, 0xAD, 0x46, 0xEA, 0x19, 0xA9, 0xD5, 0xCE, 0x93, 0xB3, 0x35, 0x73, 0x0A, 0x69, 0x59, 0x91, 0xC3, 0x0F, 0x22, 0x1B, 0x1D, 0x91, 0x13, 0x3D, 0x91, 0x73, 0x43, 0xF1, 0x6C, 0x55, 0xDA, 0x3A, 0x4F, 0xBA, 0x25, 0xCE, 0x4F, 0x04, 0xF1, 0xC5, 0xCF, 0x71, 0xDA, 0x3C, 0xD7, 0xB9, 0xB2, 0x48, 0xB4, 0x89, 0x38, 0x20, 0x4B, 0x2A, 0x95, 0x0C, 0xD5, 0xEF, 0x5B, 0xAD, 0x96, 0x45, 0x8A, 0x41, 0x96, 0x7A, 0x1F, 0x60, 0x0D, 0x7D, 0x22, 0x75, 0x82, 0x2B, 0x0F, 0xFB, 0xCE, 0x51, 0x3D, 0x2E, 0x3A, 0x21, 0xF3, 0x1C, 0xD9, 0x38, 0x86, 0x2C, 0xC6, 0x05, 0xB6, 0x7B, 0x9A, 0x8F, 0x0F, 0x97, 0x1B, 0x72, 0x6F, 0x1C, 0xEB, 0xAE, 0xFF, 0xDA, 0x97, 0x0D, 0xBA, 0x43, 0x32, 0xCA, 0x66, 0x34, 0x3D, 0x54, 0xCB, 0x24, 0x9B, 0x43, 0xF2, 0x70, 0x3E, 0x42, 0xBB, 0xA0, 0x95, 0x11, 0x37, 0x46, 0xE1, 0x4F, 0x49, 0xC5, 0x1B, 0xFC, 0x3C, 0x3A, 0x3E, 0xD1, 0x65, 0x0E, 0x6F, 0x58, 0xF8, 0x9E, 0x5B, 0xDB, 0x55, 0xB6, 0x41, 0x34, 0xCB, 0xBE, 0xDB, 0x87, 0x5F, 0xA9, 0xD1, 0x85, 0x6B, 0xB3, 0x17, 0x9C, 0x61, 0x0C, 0x9B, 0xA2, 0x5D, 0x61, 0x10, 0xED, 0x2A, 0x9B, 0xA2, 0x5D, 0x61, 0x10, 0xED, 0x2A, 0x9B, 0xA2, 0x5D, 0x61, 0x10, 0xED, 0x2A, 0x9B, 0xED, 0xC9, 0xFC, 0xDF, 0x14, 0x54, 0x8F, 0x80, 0x7A, 0x06, 0xF5, 0x23, 0xA0, 0x9F, 0x41, 0xF3, 0x10, 0x30, 0x4F, 0x41, 0xF3, 0x18, 0x30, 0xCF, 0xCA, 0xFC, 0xFF, 0x35, 0xC9, 0x79, 0xC9, 0x89, 0xFA, 0x33, 0xD7, 0x1D, 0xF6, 0x5E, 0x84, 0x5C, 0x56, 0x6E, 0xA7, 0xDA, 0x1E, 0xF9, 0xFA, 0xAB, 0xF5, 0x97, 0xFF, 0x2F, 0xED, 0x89, 0x7E, 0x29, 0x9E, 0xB4, 0x9F, 0x74, 0x1E, 0x69, 0xDA, 0xA4, 0x9F, 0x81, 0x94, 0xEF, 0x4F, 0xF6, 0xF9, 0x0B, 0xF4, 0x65, 0x51, 0x08};

void utils_welcomeScreen(EVE_HalContext *phost, char *info[])
{
	EVE_CoCmd_inflate(phost, 0, 0);
#if defined(MSVC_PLATFORM) || defined(MSVC_FT800EMU)
	EVE_Cmd_wrMem(phost, home_start_icon, sizeof(home_start_icon)); // Load from RAM
#else
	EVE_Cmd_wrProgMem(phost, home_start_icon, sizeof(home_start_icon)); // Load from Flash
#endif
	EVE_Cmd_waitFlush(phost);

	do
	{
		utils_displayStartColor(phost, (uint8_t[]){255, 255, 255}, (uint8_t[]){219, 180, 150});
		EVE_CoDl_colorRgb(phost, 0, 0, 0);
		EVE_CoCmd_text(phost, phost->Width / 2, 40, 31, OPT_CENTER, info[0]);
		EVE_CoCmd_text(phost, phost->Width / 2, 80, 31, OPT_CENTER, info[1]);
		EVE_CoCmd_text(phost, phost->Width / 2, 120, 31, OPT_CENTER, info[2]);
		EVE_CoCmd_text(phost, phost->Width / 2, 140, 31, OPT_CENTER, info[3]);
		EVE_CoCmd_text(phost, phost->Width / 2, 200, 31, OPT_CENTER, "Click to play");

		// play button circle boundary
		EVE_CoDl_colorRgb(phost, 100, 100, 100);
		EVE_CoDl_begin(phost, POINTS);
		EVE_CoDl_pointSize(phost, 50 * 16);
		EVE_CoDl_tag(phost, 'P');
		vertex2f(phost, phost->Width / 2, phost->Height / 2);
		EVE_CoDl_colorRgb(phost, 180, 35, 35);

		// play button
		EVE_CoDl_begin(phost, BITMAPS);
		EVE_CoDl_bitmapHandle(phost, 0); // handle for background stars
		EVE_CoDl_bitmapSource(phost, 0); // Starting address in ramg
		EVE_CoDl_cell(phost, 4);
		EVE_CoCmd_loadIdentity(phost);
		EVE_CoCmd_scale(phost, 65536 * 3, 65536 * 3);
		EVE_CoCmd_setMatrix(phost);
		EVE_CoDl_bitmapLayout(phost, L4, 16, 32); // format
		EVE_CoDl_bitmapSize(phost, NEAREST, BORDER, BORDER, 32 * 3, 32 * 3);
		vertex2f(phost, phost->Width / 2 - 45, phost->Height / 2 - 48);
		EVE_CoDl_end(phost);
		EVE_CoCmd_loadIdentity(phost);
		EVE_CoCmd_setMatrix(phost);
		utils_displayEnd(phost);
		EVE_sleep(10);
	} while (EVE_Hal_rd8(phost, REG_TOUCH_TAG) != 'P');
	utils_playSound(phost, 0x50, 255, 0xc0);
}
