/**
 * @file EVE_CoDl.h
 * @brief EVE's co-processor command
 *
 * The purpose of this header is to provide a simplified interface to display list instructions.
 * The functions do not match 1:1 with the display list instructions. Some instructions are combined
 * to simplify compatibility between platforms. (For example, BITMAP_SIZE and BITMAP_SIZE_H.)
 * All functions write to the display list through EVE_CoCmd_dl.
 *
 * Compatibility:
 * - EVE_CoDl_vertexFormat and EVE_CoDl_vertex2f implement fallback functionality for
 *   VERTEX_FORMAT, VERTEX_TRANSLATE_X, and VERTEX_TRANSLATE_Y on FT80X series.
 * - EVE_CoDl_bitmapSize calls BITMAP_SIZE_H on FT81X series and higher.
 * - EVE_CoDl_bitmapLayout calls BITMAP_LAYOUT_H on FT81X series and higher.
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

#ifndef EVE_CO_DL__H
#define EVE_CO_DL__H

/*********************
 *      INCLUDES
 *********************/
#include "EVE_CoCmd.h"

/**********************
 *      MACROS
 **********************/
#define EVE_VERTEX2F_MIN -16384L
#define EVE_VERTEX2F_MAX 16383L
#define EVE_VERTEX2II_MIN 0UL
#define EVE_VERTEX2II_MAX 511UL

/**********************
 *   INLINE FUNCTIONS
 **********************/
/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_display(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, DISPLAY());
}

/**
 * @brief Fixed point vertex with subprecision depending on current vertex format
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
static inline void EVE_CoDl_vertex2f(EVE_HalContext *phost, int16_t x, int16_t y)
{
	EVE_CoCmd_dl(phost, VERTEX2F(x, y));
}

/**
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 * @param handle
 * @param cell
 */
inline static void EVE_CoDl_vertex2ii(EVE_HalContext *phost, uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
{
	EVE_CoCmd_dl(phost, VERTEX2II(x, y, handle, cell));
}

/**
 * @param phost Pointer to Hal context
 * @param addr
 */
inline static void EVE_CoDl_bitmapSource(EVE_HalContext *phost, uint32_t addr)
{
	EVE_CoCmd_dl(phost, BITMAP_SOURCE(addr));
}

/**
 * @param phost Pointer to Hal context
 * @param addr
 * @param flash
 */
inline static void EVE_CoDl_bitmapSource_ex(EVE_HalContext *phost, uint32_t addr, bool flash)
{
	EVE_CoCmd_dl(phost, BITMAP_SOURCE2(flash, addr));
}

/**
 * @brief Specify clear color RGB
 *
 * @param phost Pointer to Hal context
 * @param c clear color
 */
inline static void EVE_CoDl_clearColorRgb_ex(EVE_HalContext *phost, uint32_t c)
{
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(0, 0, 0) | (c & 0xFFFFFF));
}

/**
 * @brief Specify clear color RGB
 *
 * @param phost Pointer to Hal context
 * @param r
 * @param g
 * @param b
 */
inline static void EVE_CoDl_clearColorRgb(EVE_HalContext *phost, uint8_t r, uint8_t g, uint8_t b)
{
	EVE_CoCmd_dl(phost, CLEAR_COLOR_RGB(r, g, b));
}

/**
 * @brief Specify clear alpha channel
 *
 * @param phost Pointer to Hal context
 * @param alpha
 */
inline static void EVE_CoDl_clearColorA(EVE_HalContext *phost, uint8_t alpha)
{
	EVE_CoCmd_dl(phost, CLEAR_COLOR_A(alpha));
}

/**
 * @brief Specify clear color: Alpha (bits 31:24) + RGB (bits 23:0)
 *
 * @param phost Pointer to Hal context
 * @param c clear color
 */
inline static void EVE_CoDl_clearColorArgb_ex(EVE_HalContext *phost, uint32_t c)
{
	EVE_CoDl_clearColorRgb_ex(phost, c);
	EVE_CoDl_clearColorA(phost, c >> 24);
}

/**
 * @brief Set current tag.
 *
 * @warning: Must be returned to 255 after usage, to ensure next widgets don't draw with invalid tag
 * @param phost Pointer to Hal context
 * @param s
 */
inline static void EVE_CoDl_tag(EVE_HalContext *phost, uint8_t s)
{
	EVE_CoCmd_dl(phost, TAG(s));
}

/**
 * @brief Specify color RGB
 *
 * @param phost Pointer to Hal context
 * @param c
 */
inline static void EVE_CoDl_colorRgb_ex(EVE_HalContext *phost, uint32_t c)
{
	EVE_CoCmd_dl(phost, COLOR_RGB(0, 0, 0) | (c & 0xFFFFFF));
}

/**
 * @brief Specify color RGB
 *
 * @param phost Pointer to Hal context
 * @param r
 * @param g
 * @param b
 */
inline static void EVE_CoDl_colorRgb(EVE_HalContext *phost, uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t rgb = ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b);
	EVE_CoDl_colorRgb_ex(phost, rgb);
}

/**
 * @brief Specify alpha channel
 *
 * @param phost Pointer to Hal context
 * @param alpha
 */
inline static void EVE_CoDl_colorA(EVE_HalContext *phost, uint8_t alpha)
{
	EVE_CoCmd_dl(phost, COLOR_A(alpha));
}

/**
 * @brief Specify color: Alpha (bits 31:24) + RGB (bits 23:0)
 *
 * @param phost Pointer to Hal context
 * @param c
 */
inline static void EVE_CoDl_colorArgb_ex(EVE_HalContext *phost, uint32_t c)
{
	EVE_CoDl_colorRgb_ex(phost, c);
	EVE_CoDl_colorA(phost, c >> 24);
}

/**
 * @brief Specify bitmap handle, see BITMAP_HANDLE
 *
 * @param phost Pointer to Hal context
 * @param handle
 */
inline static void EVE_CoDl_bitmapHandle(EVE_HalContext *phost, uint8_t handle)
{
	EVE_CoCmd_dl(phost, BITMAP_HANDLE(handle));
}

/**
 * @brief Specify cell number for bitmap, see CELL
 *
 * @param phost Pointer to Hal context
 * @param cell
 */
inline static void EVE_CoDl_cell(EVE_HalContext *phost, uint8_t cell)
{
	EVE_CoCmd_dl(phost, CELL(cell));
}

/**
 * @param phost Pointer to Hal context
 * @param format
 * @param linestride
 * @param height
 */
static inline void EVE_CoDl_bitmapLayout(EVE_HalContext *phost, uint8_t format, uint16_t linestride, uint16_t height)
{
	EVE_CoCmd_dl(phost, BITMAP_LAYOUT_H(linestride >> 10, height >> 9));
	EVE_CoCmd_dl(phost, BITMAP_LAYOUT(format, linestride, height));
}

/**
 * @param phost Pointer to Hal context
 * @param filter
 * @param wrapx
 * @param wrapy
 * @param width
 * @param height
 */
static inline void EVE_CoDl_bitmapSize(EVE_HalContext *phost, uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height)
{
	EVE_CoCmd_dl(phost, BITMAP_SIZE_H(width >> 9, height >> 9));
	EVE_CoCmd_dl(phost, BITMAP_SIZE(filter, wrapx, wrapy, width, height));
}

/**
 * @param phost Pointer to Hal context
 * @param func
 * @param ref
 */
static inline void EVE_CoDl_alphaFunc(EVE_HalContext *phost, uint8_t func, uint8_t ref)
{
	EVE_CoCmd_dl(phost, ALPHA_FUNC(func, ref));
}

/**
 * @param phost Pointer to Hal context
 * @param func
 * @param ref
 * @param mask
 */
static inline void EVE_CoDl_stencilFunc(EVE_HalContext *phost, uint8_t func, uint8_t ref, uint8_t mask)
{
	EVE_CoCmd_dl(phost, STENCIL_FUNC(func, ref, mask));
}

/**
 * @param phost Pointer to Hal context
 * @param src
 * @param dst
 */
static inline void EVE_CoDl_blendFunc(EVE_HalContext *phost, uint8_t src, uint8_t dst)
{
	EVE_CoCmd_dl(phost, BLEND_FUNC(src, dst));
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_blendFunc_default(EVE_HalContext *phost)
{
	EVE_CoDl_blendFunc(phost, SRC_ALPHA, ONE_MINUS_SRC_ALPHA); // TODO: Add default calls for all display list state instructions
}

/**
 * @param phost Pointer to Hal context
 * @param sfail
 * @param spass
 */
static inline void EVE_CoDl_stencilOp(EVE_HalContext *phost, uint8_t sfail, uint8_t spass)
{
	EVE_CoCmd_dl(phost, STENCIL_OP(sfail, spass));
}

/**
 * @param phost Pointer to Hal context
 * @param size
 */
inline static void EVE_CoDl_pointSize(EVE_HalContext *phost, int16_t size)
{
	EVE_CoCmd_dl(phost, POINT_SIZE(size));
}

/**
 * @param phost Pointer to Hal context
 * @param width
 */
inline static void EVE_CoDl_lineWidth(EVE_HalContext *phost, int16_t width)
{
	EVE_CoCmd_dl(phost, LINE_WIDTH(width));
}

/**
 * @param phost Pointer to Hal context
 * @param s
 */
static inline void EVE_CoDl_clearStencil(EVE_HalContext *phost, uint8_t s)
{
	EVE_CoCmd_dl(phost, CLEAR_STENCIL(s));
}

/**
 * @param phost Pointer to Hal context
 * @param s
 */
static inline void EVE_CoDl_clearTag(EVE_HalContext *phost, uint8_t s)
{
	EVE_CoCmd_dl(phost, CLEAR_TAG(s));
}

/**
 * @param phost Pointer to Hal context
 * @param mask
 */
static inline void EVE_CoDl_stencilMask(EVE_HalContext *phost, uint8_t mask)
{
	EVE_CoCmd_dl(phost, STENCIL_MASK(mask));
}

/**
 * @param phost Pointer to Hal context
 * @param mask
 */
static inline void EVE_CoDl_tagMask(EVE_HalContext *phost, bool mask)
{
	EVE_CoCmd_dl(phost, TAG_MASK(mask));
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformA(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_A(v));
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformA_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_A_EXT(p, v));
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformB(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_B(v));
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformB_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_B_EXT(p, v));
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformC(EVE_HalContext *phost, uint32_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_C(v));
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformD(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_D(v));
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformD_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_D_EXT(p, v));
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformE(EVE_HalContext *phost, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_E(v));
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformE_ex(EVE_HalContext *phost, bool p, uint16_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_E_EXT(p, v));
}

/**
 * @param phost Pointer to Hal context
 * @param v
 */
static inline void EVE_CoDl_bitmapTransformF(EVE_HalContext *phost, uint32_t v)
{
	EVE_CoCmd_dl(phost, BITMAP_TRANSFORM_F(v));
}

/**
 * @param phost Pointer to Hal context
 * @param p
 * @param a
 * @param b
 * @param c
 * @param d
 * @param e
 * @param f
 */
static inline void EVE_CoDl_bitmapTransform_ex(EVE_HalContext *phost, bool p, uint16_t a, uint16_t b, uint32_t c, uint16_t d, uint16_t e, uint32_t f)
{
	EVE_CoDl_bitmapTransformA_ex(phost, p, a);
	EVE_CoDl_bitmapTransformB_ex(phost, p, b);
	EVE_CoDl_bitmapTransformC(phost, c);
	EVE_CoDl_bitmapTransformD_ex(phost, p, d);
	EVE_CoDl_bitmapTransformE_ex(phost, p, e);
	EVE_CoDl_bitmapTransformF(phost, f);
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_bitmapTransform_identity(EVE_HalContext *phost)
{
	EVE_CoDl_bitmapTransformA_ex(phost, 0, 256);
	EVE_CoDl_bitmapTransformB_ex(phost, 0, 0);
	EVE_CoDl_bitmapTransformC(phost, 0);
	EVE_CoDl_bitmapTransformD_ex(phost, 0, 0);
	EVE_CoDl_bitmapTransformE_ex(phost, 0, 256);
	EVE_CoDl_bitmapTransformF(phost, 0);
}

/**
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
static inline void EVE_CoDl_scissorXY(EVE_HalContext *phost, uint16_t x, uint16_t y)
{
	EVE_CoCmd_dl(phost, SCISSOR_XY(x, y));
}

/**
 * @param phost Pointer to Hal context
 * @param width
 * @param height
 */
static inline void EVE_CoDl_scissorSize(EVE_HalContext *phost, uint16_t width, uint16_t height)
{
	EVE_CoCmd_dl(phost, SCISSOR_SIZE(width, height));
}

/**
 * @param phost Pointer to Hal context
 * @param dest
 */
static inline void EVE_CoDl_call(EVE_HalContext *phost, uint16_t dest)
{
	EVE_CoCmd_dl(phost, CALL(dest));
}

/**
 * @param phost Pointer to Hal context
 * @param dest
 */
static inline void EVE_CoDl_jump(EVE_HalContext *phost, uint16_t dest)
{
	EVE_CoCmd_dl(phost, JUMP(dest));
}

/**
 * @param phost Pointer to Hal context
 * @param prim
 */
static inline void EVE_CoDl_begin(EVE_HalContext *phost, uint8_t prim)
{
	EVE_CoCmd_dl(phost, BEGIN(prim));
}

/**
 * @param phost Pointer to Hal context
 * @param r
 * @param g
 * @param b
 * @param a
 */
static inline void EVE_CoDl_colorMask(EVE_HalContext *phost, bool r, bool g, bool b, bool a)
{
	EVE_CoCmd_dl(phost, COLOR_MASK(r, g, b, a));
}

/**
 * @param phost Pointer to Hal context
 */
inline static void EVE_CoDl_end(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, END());
}

/**
 * @brief Save EVE context, see SAVE_CONTEXT
 *
 * @param phost Pointer to Hal context
 */
inline static void EVE_CoDl_saveContext(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, SAVE_CONTEXT());
}

/**
 * @brief Restore EVE context, see RESTORE_CONTEXT
 *
 * @param phost Pointer to Hal context
 */
inline static void EVE_CoDl_restoreContext(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, RESTORE_CONTEXT());
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_return(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, RETURN());
}

/**
 * @param phost Pointer to Hal context
 * @param m
 */
static inline void EVE_CoDl_macro(EVE_HalContext *phost, uint16_t m)
{
	EVE_CoCmd_dl(phost, MACRO(m));
}

/**
 * @param phost Pointer to Hal context
 * @param c clear color
 * @param s clear stencil
 * @param t clear tag
 */
static inline void EVE_CoDl_clear(EVE_HalContext *phost, bool c, bool s, bool t)
{
	EVE_CoCmd_dl(phost, CLEAR(c, s, t));
}

/**
 * @param phost Pointer to Hal context
 * @param frac
 */
static inline void EVE_CoDl_vertexFormat(EVE_HalContext *phost, uint8_t frac)
{
	EVE_CoCmd_dl(phost, VERTEX_FORMAT(frac));
}

/**
 * @brief Set palette source, see PALETTE_SOURCE command
 *
 * @param phost Pointer to Hal context
 * @param addr
 */
inline static void EVE_CoDl_paletteSource(EVE_HalContext *phost, uint32_t addr)
{
	EVE_CoCmd_dl(phost, PALETTE_SOURCE(addr));
}

/**
 * @param phost Pointer to Hal context
 * @param x
 */
static inline void EVE_CoDl_vertexTranslateX(EVE_HalContext *phost, int16_t x)
{
	EVE_CoCmd_dl(phost, VERTEX_TRANSLATE_X(x));
}

/**
 * @param phost Pointer to Hal context
 * @param y
 */
static inline void EVE_CoDl_vertexTranslateY(EVE_HalContext *phost, int16_t y)
{
	EVE_CoCmd_dl(phost, VERTEX_TRANSLATE_Y(y));
}

/**
 * @param phost Pointer to Hal context
 */
static inline void EVE_CoDl_nop(EVE_HalContext *phost)
{
	EVE_CoCmd_dl(phost, NOP());
}

/**
 * @param phost Pointer to Hal context
 * @param format
 */
static inline void EVE_CoDl_bitmapExtFormat(EVE_HalContext *phost, uint16_t format)
{
	EVE_CoCmd_dl(phost, BITMAP_EXT_FORMAT(format));
}

/**
 * @param phost Pointer to Hal context
 * @param r
 * @param g
 * @param b
 * @param a
 */
static inline void EVE_CoDl_bitmapSwizzle(EVE_HalContext *phost, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	EVE_CoCmd_dl(phost, BITMAP_SWIZZLE(r, g, b, a));
}

/**
 * @brief Fixed point vertex using 4 bits subprecision
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
inline static void EVE_CoDl_vertex2f_4(EVE_HalContext *phost, int16_t x, int16_t y)
{
	if (x > EVE_VERTEX2F_MAX || y > EVE_VERTEX2F_MAX) /* Support display up to 2048 px */
	{
		EVE_CoDl_vertexFormat(phost, 3);
		EVE_CoDl_vertex2f(phost, x >> 1, y >> 1);
	}
	else
	{
		EVE_CoDl_vertexFormat(phost, 4);
		EVE_CoDl_vertex2f(phost, x, y);
	}
}

/**
 * @brief Fixed point vertex using 2 bits subprecision
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
inline static void EVE_CoDl_vertex2f_2(EVE_HalContext *phost, int16_t x, int16_t y)
{
	EVE_CoDl_vertexFormat(phost, 2);
	EVE_CoDl_vertex2f(phost, x, y);
}

/**
 * @brief Fixed point vertex using 0 bits subprecision, or integer point vertex
 *
 * @param phost Pointer to Hal context
 * @param x
 * @param y
 */
inline static void EVE_CoDl_vertex2f_0(EVE_HalContext *phost, int16_t x, int16_t y)
{
	EVE_CoDl_vertexFormat(phost, 0);
	EVE_CoDl_vertex2f(phost, x, y);
}

#endif /* EVE_CO_DL__H */

/* end of file */
