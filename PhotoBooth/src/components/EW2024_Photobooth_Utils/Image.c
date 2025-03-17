/**
 * @file Image.c
 /**
 * @file Image.c
 * @brief Utility functions for processing image formats
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "EW2024_Photobooth_Utils.h"
#include "Draw.h"

static uint32_t utils_getFormatW(uint32_t format)
{
	switch (format)
	{
	case COMPRESSED_RGBA_ASTC_4x4_KHR:
		return 4;
	case COMPRESSED_RGBA_ASTC_5x4_KHR:
		return 5;
	case COMPRESSED_RGBA_ASTC_5x5_KHR:
		return 5;
	case COMPRESSED_RGBA_ASTC_6x5_KHR:
		return 6;
	case COMPRESSED_RGBA_ASTC_6x6_KHR:
		return 6;
	case COMPRESSED_RGBA_ASTC_8x5_KHR:
		return 8;
	case COMPRESSED_RGBA_ASTC_8x6_KHR:
		return 8;
	case COMPRESSED_RGBA_ASTC_8x8_KHR:
		return 8;
	case COMPRESSED_RGBA_ASTC_10x5_KHR:
		return 10;
	case COMPRESSED_RGBA_ASTC_10x6_KHR:
		return 10;
	case COMPRESSED_RGBA_ASTC_10x8_KHR:
		return 10;
	case COMPRESSED_RGBA_ASTC_10x10_KHR:
		return 10;
	case COMPRESSED_RGBA_ASTC_12x10_KHR:
		return 12;
	case COMPRESSED_RGBA_ASTC_12x12_KHR:
		return 12;
	default:
		return 4;
	}
}

static uint32_t utils_getFormatH(uint32_t format)
{
	switch (format)
	{
	case COMPRESSED_RGBA_ASTC_4x4_KHR:
		return 4;
	case COMPRESSED_RGBA_ASTC_5x4_KHR:
		return 4;
	case COMPRESSED_RGBA_ASTC_5x5_KHR:
		return 5;
	case COMPRESSED_RGBA_ASTC_6x5_KHR:
		return 5;
	case COMPRESSED_RGBA_ASTC_6x6_KHR:
		return 6;
	case COMPRESSED_RGBA_ASTC_8x5_KHR:
		return 5;
	case COMPRESSED_RGBA_ASTC_8x6_KHR:
		return 6;
	case COMPRESSED_RGBA_ASTC_8x8_KHR:
		return 8;
	case COMPRESSED_RGBA_ASTC_10x5_KHR:
		return 5;
	case COMPRESSED_RGBA_ASTC_10x6_KHR:
		return 6;
	case COMPRESSED_RGBA_ASTC_10x8_KHR:
		return 8;
	case COMPRESSED_RGBA_ASTC_10x10_KHR:
		return 10;
	case COMPRESSED_RGBA_ASTC_12x10_KHR:
		return 10;
	case COMPRESSED_RGBA_ASTC_12x12_KHR:
		return 12;
	default:
		return 4;
	}
}
uint32_t utils_setImage(EVE_HalContext *phost, Image *image, uint32_t palette)
{
	Begin(phost, BITMAPS);
	bitmap_handle(phost, (uint16_t)image->bitmap_handler);

	if (palette != 0)
	{
		PaletteSource(phost, palette);
	}
	else if (image->bitmap_format == PALETTEDARGB8)
	{
		PaletteSource(phost, image->ptr);
		palette_sourceh2(phost, image->ptr);
	}

	EVE_CoCmd_setBitmap(phost, image->ptr, image->bitmap_format, image->w, image->h);

	// image scaling need a bigger bitmap size
	if (image->scale != 100 && 0 < image->scale && image->scale < 1000)
	{
		uint32_t frameW = image->w * image->scale / 100;
		uint32_t frameH = image->h * image->scale / 100;
		bitmap_size(PHOST, NEAREST, BORDER, BORDER, min(511, frameW), min(511, frameH));
		bitmap_size_h(PHOST, frameW >> 9, frameH >> 9);
	}

	return 0;
}

static uint32_t utils_loadRawFromSdCard(EVE_HalContext *phost, Image *image)
{
	uint32_t image_size = 0;
	image->is_loaded = 0;
	cmd_fssize(phost, (char *)image->sd_path, &image_size);
	finish(phost);

	if (image_size == (uint32_t)-1)
	{
		APP_INF("cmd_fssize error: File not found: %s", (char *)image->sd_path);
		return 1;
	}

	if (image_size == 0)
	{
		APP_INF("cmd_fssize error: image size= 0");
		return 2;
	}

	uint32_t load_image_offset = utils_ddrAllocAlignment(image_size, 32);
	uint32_t result = 0;
	cmd_fsread(phost, load_image_offset, (char *)image->sd_path, &result);
	finish(phost);
	image->is_loaded = 1;

#define ENOENT 2
#define EIO 5
	if (result != 0)
	{
		if (result == ENOENT)
		{
			APP_INF("cmd_fsread error: file not found");
		}
		else if (result == EIO)
		{
			APP_INF("cmd_fsread error: IO error");
		}
		else
		{
			APP_INF("cmd_fsread error: Unexpected error");
		}
		return 0;
	}
	image->ptr = load_image_offset;
	uint32_t palette = 0;

	if (image->bitmap_handler == BMHL_AUTO)
	{
		image->bitmap_handler = utils_BitmapHandler_get();
	}

	if (IS_BITMAP_HNDLER_VALID(image->bitmap_handler))
	{
		utils_setImage(phost, image, palette);
	}
	return 0;
}
uint32_t utils_loadImageFromSdCard(EVE_HalContext *phost, Image *image)
{
	if (image->fmt == IMG_FMT_RAW)
	{
		return utils_loadRawFromSdCard(phost, image);
	}

	image->is_loaded = 0;

	uint32_t result = 0;
	cmd_fssource(phost, (char *)image->sd_path, &result);
	finish(phost);
	if (result != 0)
	{
		if (result == 2)
		{
			APP_INF("cmd_fssource error: file not found");
		}
		else if (result == 5)
		{
			APP_INF("cmd_fssource error: IO error");
		}
		else
		{
			APP_INF("cmd_fssource error: Unexpected error");
		}
		return 3;
	}

	uint32_t image_size = 0;
	cmd_fssize(phost, (char *)image->sd_path, &image_size);
	finish(phost);
	if (image_size == (uint32_t)-1)
	{
		APP_INF("cmd_fssize error: File not found: %s", (char *)image->sd_path);
		return 1;
	}

	if (image_size == 0)
	{
		APP_INF("cmd_fssize error: image size= 0");
		return 2;
	}

	uint32_t load_image_offset = utils_ddrAlloc(0);
	cmd_loadimage(phost, load_image_offset, image->opt | OPT_FS | OPT_NODL | OPT_DITHER);
	finish(phost);
	image->is_loaded = 1;

	uint32_t w = 0, h = 0, img_addr = 0;
	cmd_getprops(phost, &img_addr, &w, &h);
	if (w != 0)
	{
		image->w = w;
	}
	if (h != 0)
	{
		image->h = h;
	}
	if (img_addr == 0)
	{
		image->ptr = load_image_offset;
	}
	else
	{
		image->ptr = img_addr;
	}
	// TODO: allocate memory based on image actual size with different format
	utils_ddrAlloc(image->w * image->h * 2);
	finish(phost);

	uint32_t bitmapEndPtr = 0;
	cmd_getptr(phost, &bitmapEndPtr);
	finish(phost);

	uint32_t source = 0;
	uint32_t fmt = 0;
	w = 0;
	h = 0;
	uint32_t palette;
	cmd_getimage(phost, &source, &fmt, &w, &h, &palette);
	image->bitmap_format = fmt;
	finish(phost);

	if (image->bitmap_handler == BMHL_AUTO)
	{
		image->bitmap_handler = utils_BitmapHandler_get();
	}

	image->tagval = utils_tagAlloc(image->tagval);

	if (bitmapEndPtr > 0)
	{
		load_image_offset = bitmapEndPtr;
	}
	else
	{
		load_image_offset += (image->w + image->h) * 2;
	}

	load_image_offset = (load_image_offset + 31) & ~31;

	if (IS_BITMAP_HNDLER_VALID(image->bitmap_handler))
	{
		utils_setImage(phost, image, palette);
	}
	printf("Loaded image %s \n", (char *)image->sd_path);
	return 0;
}

uint32_t utils_drawImage(EVE_HalContext *phost, Image *image)
{
	return utils_drawImageXY(phost, image, image->x, image->y);
}

uint32_t utils_drawImageCenter(EVE_HalContext *phost, Image *image, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	uint32_t ix = x + w / 2 - image->w / 2;
	uint32_t iy = y + h / 2 - image->h / 2;

	return utils_drawImageXY(phost, image, ix, iy);
}

uint32_t utils_drawImageXyTag(EVE_HalContext *phost, Image *image, uint32_t x, uint32_t y, uint32_t tag)
{
	if (image->is_loaded == 0)
	{
		return 1;
	}

	Tag(phost, tag);

	begin(phost, BITMAPS);
	if (IS_BITMAP_HNDLER_VALID(image->bitmap_handler))
	{
		bitmap_handle_set(phost, (uint16_t)image->bitmap_handler);
	}
	else
	{
		if (image->scale > 100)
			save_context(PHOST);
		utils_setImage(phost, image, 0);
		if (image->scale > 100)
			restore_context(PHOST);
	}
	vertex2f(phost, x, y);
}

uint32_t utils_drawImageXY(EVE_HalContext *phost, Image *image, uint32_t x, uint32_t y)
{
	return utils_drawImageXyTag(phost, image, x, y, image->tagval);
}