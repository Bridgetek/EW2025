/**
 * @file EW2024_Photobooth_Utils.h
 * @brief Utility functions and definitions for the EW2024 Photobooth application.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef EW2024_PHOTOBOOTH_UTILS_H_
#define EW2024_PHOTOBOOTH_UTILS_H_

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "EVE_Hal.h"
#include "EVE_HalImpl.h"
#include "EVE_Platform.h"

#include "Gesture.h"
#include "Scanout.h"
#include "Timer.h"
#include "Tags.h"
#include "Draw.h"
#include "Image.h"
#include "Bitmap_Handler.h"
#include "Ddr.h"
#include "phost.h"
#include "Paging.h"

#define utils_calibrateNew(phost) utils_calibrateInit(phost, 0, 0, 0, 0, 0, 0)

#define REG_LVDSRX_SYS_SETUP 0x7F800500
#define REG_LVDSRX_SYS_CTRL 0x7F800504

/** macro: PRINTF_LEVEL
 *    range: 0-3
 *    0: Disable printf
 *    1: Only APP_INF messages and printf function calls
 *    2: Only APP_ERR messages
 *    3: APP_ERR messages and APP_DBG messages
 */
#define PRINTF_LEVEL 3
#define NOTE(...)
#define APP_PRINTF_INFO(M, ...) printf("[INF] " M "\r\n", ##__VA_ARGS__)
#define APP_PRINTF_ERROR(M, ...) printf("[ERROR] %s:%d: " M "\r\n", __func__, __LINE__, ##__VA_ARGS__)
#define APP_PRINTF_DEBUG(M, ...) printf("[DBG] %s:%s:%d: " M "\r\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#if PRINTF_LEVEL == 1
#define APP_INF APP_PRINTF_INFO
#define APP_ERR(M, ...) /* nothing */
#define APP_DBG(M, ...) /* nothing */
#elif PRINTF_LEVEL == 2
#define APP_INF APP_PRINTF_INFO
#define APP_ERR APP_PRINTF_ERROR
#define APP_DBG(M, ...) /* nothing */
#elif PRINTF_LEVEL == 3
#define APP_INF APP_PRINTF_INFO
#define APP_ERR APP_PRINTF_ERROR
#define APP_DBG APP_PRINTF_DEBUG
#else
#define APP_INF(M, ...) /* nothing */
#define APP_ERR(M, ...) /* nothing */
#define APP_DBG(M, ...) /* nothing */
#define printf(M, ...)  /* nothing */
#endif
#define APP_DBG_H2(x) APP_DBG(#x ": 0x%02x", (x))
#define APP_DBG_H8(x) APP_DBG(#x ": 0x%08x", (x))
#define APP_DBG_D(x) APP_DBG(#x ": %d", (int)(x))
#define APP_DBG_U(x) APP_DBG(#x ": %u", (unsigned int)(x))
#define APP_DBG_F8(x) APP_DBG(#x ": 0x%08f", (float)(x))
#define APP_DBG_F2(x) APP_DBG(#x ": 0x%02f", (float)(x))
#define APP_DBG_S(x) APP_DBG(#x ": %s", (char *)(x))
#define INPUT_YES 1
#define INPUT_NO 0
#define APP_PRINTF printf // for different with printf in a library, 3rd source...etc

#define utils_drawTextFormat(phost, M, ...)    \
    {                                          \
        sprintf(textbuffer, M, ##__VA_ARGS__); \
        utils_drawText(phost, textbuffer);     \
    }

#define utils_drawTextFormat2(phost, ms, M, ...) \
    {                                            \
        sprintf(textbuffer, M, ##__VA_ARGS__);   \
        utils_drawText2(phost, textbuffer, ms);  \
    }

/**********************
 *  EXTERN VARIABLES
 **********************/
extern char textbuffer[1000];

#define LOADIMAGE 1 /**< loadimage command takes destination address and options before the actual bitmap data */
#define INFLATE 2   /**< inflate command takes destination address before the actual bitmap */
#define LOAD 3      /**< load bitmaps directly */
#define INFLATE2 4

#define PROGMEM eve_progmem
#define PROGMEM_CONST eve_progmem_const

#ifndef MIN
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#endif
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

// quater lcd
// #define REG_LVDSRX_SYS_SETUP_VALUE   0x17
// #define REG_LVDSRX_SYS_CTRL_VALUE    0x8B8B

// half lcd
#define REG_LVDSRX_SYS_SETUP_VALUE 0x16
#define REG_LVDSRX_SYS_CTRL_VALUE 0x8989

#define SC0_PTR0_STARTADDR (36 << 20) // from 36M	(size: 9M)
#define SC0_PTR1_STARTADDR (45 << 20) // from 45M	(size: 9M)
#define SC1_PTR0_STARTADDR (54 << 20) // from 54M	(size: 9M)
#define SC1_PTR1_STARTADDR (63 << 20) // from 63M	(size: 9M)
#define SC2_PTR0_STARTADDR (72 << 20) // from 72M	(size: 9M)
#define SC2_PTR1_STARTADDR (81 << 20) // from 81M	(size: 9M)

#define DDR_FRAMEBUFFER_STARTADDR (72 << 20) // from 42M (size: 9M)
#define DDR_WORKAREA (90 << 20)              // from 90M (size: 30M)

#define TESTCASE_TESTCARD 0
#define TESTCASE_PICTURE 1
#define TESTCASE_SWAPPING 2
#define TESTCASE_DIRECTVIDEO 3
#define TESTCASE_VIDEO 4
#define TESTCASE_LVDSRX 5
#define TESTCASE_MOCK_LVDSRX 6
#define TESTCASE_PLAYBACK 7
#define TESTCASE_TOUCH 8

typedef struct
{
    uint32_t format_value;
    char *format_name;
    uint32_t bits_per_pixel;
} format_info_t;

typedef struct
{
    uint32_t w, h, linestride; // linestride = width ∗ byte/pixel
    uint32_t eve_ramg_ptr;
    uint32_t memory_size;
    uint32_t bitmap_handler;
    uint32_t tagval;
    uint32_t bitmap_fmt;
    uint32_t bits_per_pixel; // See Table 22 – Bitmap formats and bits per pixel
    union
    {
        uint32_t color32;
        uint8_t color8[3];
    };
    uint8_t line_size;
    int sketch_width_id;
    bool isActive;
    char name[20];
} sketch_t;
extern format_info_t format_info_table[];

typedef struct
{
    int32_t x, y, w, h;
} box;

bool utils_calibrate(EVE_HalContext *phost);
bool utils_calibrateInit(EVE_HalContext *phost, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void utils_scale(int percent);
void utils_drawBox(box *b);
void utils_drawBoxAt(box *b, int32_t x, int32_t y);
void utils_drawRect(int32_t x, int32_t y, int32_t w, int32_t h);

format_info_t *utils_searchFormatInfo(uint32_t format_value);
uint32_t utils_searchBitsPerPixel(uint32_t bitmap_fmt);

void utils_gpuInit(EVE_HalContext *phost);
void utils_gpuRelease(EVE_HalContext *phost);
uint32_t utils_getSystemClock(EVE_HalContext *phost);
uint32_t utils_drawPoint(EVE_HalContext *phost, uint32_t Rx, uint32_t Ry, uint32_t R);
void utils_loadImageToRamg(EVE_HalContext *phost, const char *fileName, int32_t destination, uint8_t type);

void utils_calibrateRestore(EVE_HalContext *phost);
void utils_calibrateSave(EVE_HalContext *phost);
void utils_calibrateDump(EVE_HalContext *phost);
void utils_displayStart(EVE_HalContext *phost);
void utils_displayStartColor(EVE_HalContext *phost, uint8_t *bgColor, uint8_t *textColor);
void utils_displayEnd(EVE_HalContext *phost);
void utils_drawTextColor(EVE_HalContext *phost, const char *str, uint8_t *bgColor, uint8_t *textColor);
void utils_drawText(EVE_HalContext *phost, const char *str);
void utils_drawText2(EVE_HalContext *phost, const char *str, int ms);
void utils_playMuteSound(EVE_HalContext *phost);
void utils_playSound(EVE_HalContext *phost, uint8_t sound, uint8_t vol, uint8_t midi);
uint8_t utils_showDialogYesno(EVE_HalContext *phost, const char *title, const char *msg);
void utils_showDialogInfo(EVE_HalContext *phost, const uint8_t *msg);
uint8_t utils_isSDcardDetected();
void utils_hintScreen(EVE_HalContext *phost, const char *msg);
void Flash_Init(EVE_HalContext *phost, const uint8_t *filePath, const uint8_t *fileName);
void utils_welcomeScreen(EVE_HalContext *phost, char *info[]);
void utils_lvdsConfig(EVE_HalContext *phost, uint16_t format, uint8_t testcase);
void utils_lvdsScanoutSingle(EVE_HalContext *phost);
void utils_lvdsScanoutSwaping(EVE_HalContext *phost);

#define swap()                                 \
    {                                          \
        EVE_CoDl_display(s_pHalContext);       \
        EVE_CoCmd_swap(s_pHalContext);         \
        EVE_Cmd_waitFlush(s_pHalContext);      \
        cmd_graphicsfinish();                  \
        EVE_CoCmd_dlStart(s_pHalContext);      \
        EVE_CoCmd_loadIdentity(s_pHalContext); \
    }

// DL command
/// Co-processor
#define cmd_regwrite(phost, ...) EVE_CoCmd_regWrite(phost, __VA_ARGS__)
#define finish(phost) EVE_Cmd_waitFlush(phost)

// DL command
/* Mapping from CircuitPython code to C code */
#define AlphaFunc alpha_func                       // #define alpha_func(phost, ...)             cmd(ALPHA_FUNC             (__VA_ARGS__))
#define Begin begin                                // #define begin(phost, ...)                  cmd(BEGIN                  (__VA_ARGS__))
#define BitmapAddrMask bitmap_addr_mask            // #define bitmap_addr_mask(phost, ...)       cmd(BITMAP_ADDR_MASK       (__VA_ARGS__))
#define BitmapSource2 bitmap_source2               // #define bitmap_source2(phost, ...)         cmd(BITMAP_SOURCE2         (__VA_ARGS__))
#define BitmapSourceh bitmap_sourceh               // #define bitmap_sourceh(phost, ...)         cmd(BITMAP_SOURCEH         (__VA_ARGS__))
#define BitmapTransformAEx bitmap_transform_a_ex   // #define bitmap_transform_a_ex(phost, ...)  cmd(BITMAP_TRANSFORM_A_EX  (__VA_ARGS__))
#define BitmapTransformBEx bitmap_transform_b_ex   // #define bitmap_transform_b_ex(phost, ...)  cmd(BITMAP_TRANSFORM_B_EX  (__VA_ARGS__))
#define BitmapTransformDEx bitmap_transform_d_ex   // #define bitmap_transform_d_ex(phost, ...)  cmd(BITMAP_TRANSFORM_D_EX  (__VA_ARGS__))
#define BitmapTransformEEx bitmap_transform_e_ex   // #define bitmap_transform_e_ex(phost, ...)  cmd(BITMAP_TRANSFORM_E_EX  (__VA_ARGS__))
#define BitmapZorder bitmap_zorder                 // #define bitmap_zorder(phost, ...)          cmd(BITMAP_ZORDER          (__VA_ARGS__))
#define BitmapExtFormat bitmap_ext_format          // #define bitmap_ext_format(phost, ...)      cmd(BITMAP_EXT_FORMAT      (__VA_ARGS__))
#define BitmapHandle bitmap_handle                 // #define bitmap_handle(phost, ...)          cmd(BITMAP_HANDLE          (__VA_ARGS__))
#define BitmapLayout bitmap_layout                 // #define bitmap_layout(phost, ...)          cmd(BITMAP_LAYOUT          (__VA_ARGS__))
#define BitmapLayoutH bitmap_layout_h              // #define bitmap_layout_h(phost, ...)        cmd(BITMAP_LAYOUT_H        (__VA_ARGS__))
#define BitmapSize bitmap_size                     // #define bitmap_size(phost, ...)            cmd(BITMAP_SIZE            (__VA_ARGS__))
#define BitmapSizeH bitmap_size_h                  // #define bitmap_size_h(phost, ...)          cmd(BITMAP_SIZE_H          (__VA_ARGS__))
#define BitmapSource bitmap_source                 // #define bitmap_source(phost, ...)          cmd(BITMAP_SOURCE          (__VA_ARGS__))
#define BitmapSource2 bitmap_source2               // #define bitmap_source2(phost, ...)         cmd(BITMAP_SOURCE2         (__VA_ARGS__))
#define BitmapSourceh bitmap_sourceh               // #define bitmap_sourceh(phost, ...)         cmd(BITMAP_SOURCEH         (__VA_ARGS__))
#define BitmapSwizzle bitmap_swizzle               // #define bitmap_swizzle(phost, ...)         cmd(BITMAP_SWIZZLE         (__VA_ARGS__))
#define BitmapTransformA bitmap_transform_a        // #define bitmap_transform_a(phost, ...)     cmd(BITMAP_TRANSFORM_A     (__VA_ARGS__))
#define BitmapTransformAExt bitmap_transform_a_ext // #define bitmap_transform_a_ext(phost, ...) cmd(BITMAP_TRANSFORM_A_EXT (__VA_ARGS__))
#define BitmapTransformB bitmap_transform_b        // #define bitmap_transform_b(phost, ...)     cmd(BITMAP_TRANSFORM_B     (__VA_ARGS__))
#define BitmapTransformBExt bitmap_transform_b_ext // #define bitmap_transform_b_ext(phost, ...) cmd(BITMAP_TRANSFORM_B_EXT (__VA_ARGS__))
#define BitmapTransformC bitmap_transform_c        // #define bitmap_transform_c(phost, ...)     cmd(BITMAP_TRANSFORM_C     (__VA_ARGS__))
#define BitmapTransformD bitmap_transform_d        // #define bitmap_transform_d(phost, ...)     cmd(BITMAP_TRANSFORM_D     (__VA_ARGS__))
#define BitmapTransformDExt bitmap_transform_d_ext // #define bitmap_transform_d_ext(phost, ...) cmd(BITMAP_TRANSFORM_D_EXT (__VA_ARGS__))
#define BitmapTransformE bitmap_transform_e        // #define bitmap_transform_e(phost, ...)     cmd(BITMAP_TRANSFORM_E     (__VA_ARGS__))
#define BitmapTransformEExt bitmap_transform_e_ext // #define bitmap_transform_e_ext(phost, ...) cmd(BITMAP_TRANSFORM_E_EXT (__VA_ARGS__))
#define BitmapTransformF bitmap_transform_f        // #define bitmap_transform_f(phost, ...)     cmd(BITMAP_TRANSFORM_F     (__VA_ARGS__))
#define BitmapZorder bitmap_zorder                 // #define bitmap_zorder(phost, ...)          cmd(BITMAP_ZORDER          (__VA_ARGS__))
#define BlendFunc blend_func                       // #define blend_func(phost, ...)             cmd(BLEND_FUNC             (__VA_ARGS__))
#define Call call                                  // #define call(phost, ...)                   cmd(CALL                   (__VA_ARGS__))
#define Cell cell                                  // #define cell(phost, ...)                   cmd(CELL                   (__VA_ARGS__))
#define Clear clear                                // #define clear(phost, ...)                  cmd(CLEAR                  (__VA_ARGS__))
#define ClearColorA clear_color_a                  // #define clear_color_a(phost, ...)          cmd(CLEAR_COLOR_A          (__VA_ARGS__))
#define ClearColorRGB clear_color_rgb              // #define clear_color_rgb(phost, ...)        cmd(CLEAR_COLOR_RGB        (__VA_ARGS__))
#define ClearStencil clear_stencil                 // #define clear_stencil(phost, ...)          cmd(CLEAR_STENCIL          (__VA_ARGS__))
#define ClearTag clear_tag                         // #define clear_tag(phost, ...)              cmd(CLEAR_TAG              (__VA_ARGS__))
#define ColorA color_a                             // #define color_a(phost, ...)                cmd(COLOR_A                (__VA_ARGS__))
#define ColorMask color_mask                       // #define color_mask(phost, ...)             cmd(COLOR_MASK             (__VA_ARGS__))
#define ColorRGB color_rgb                         // #define color_rgb(phost, ...)              cmd(COLOR_RGB              (__VA_ARGS__))
#define Display display                            // #define display(phost, ...)                cmd(DISPLAY                (__VA_ARGS__))
#define End end                                    // #define end(phost, ...)                    cmd(END                    (__VA_ARGS__))
#define Jump jump                                  // #define jump(phost, ...)                   cmd(JUMP                   (__VA_ARGS__))
#define LineWidth line_width                       // #define line_width(phost, ...)             cmd(LINE_WIDTH             (__VA_ARGS__))
#define Macro macro                                // #define macro(phost, ...)                  cmd(MACRO                  (__VA_ARGS__))
#define Nop nop                                    // #define nop(phost, ...)                    cmd(NOP                    (__VA_ARGS__))
#define PaletteSource palette_source               // #define palette_source(phost, ...)         cmd(PALETTE_SOURCE         (__VA_ARGS__))
#define PaletteSourceh palette_sourceh             // #define palette_sourceh(phost, ...)        cmd(PALETTE_SOURCEH        (__VA_ARGS__))
#define PointSize point_size                       // #define point_size(phost, ...)             cmd(POINT_SIZE             (__VA_ARGS__))
#define RestoreContext restore_context             // #define restore_context()           cmd(RESTORE_CONTEXT        ())
#define Return return_                             // #define return_(phost, ...)                cmd(RETURN                 (__VA_ARGS__))
#define SaveContext save_context                   // #define save_context(phost, ...)           cmd(SAVE_CONTEXT           ())
#define ScissorSize scissor_size                   // #define scissor_size(phost, ...)           cmd(SCISSOR_SIZE           (__VA_ARGS__))
#define ScissorXy scissor_xy                       // #define scissor_xy(phost, ...)             cmd(SCISSOR_XY             (__VA_ARGS__))
#define StencilFunc stencil_func                   // #define stencil_func(phost, ...)           cmd(STENCIL_FUNC           (__VA_ARGS__))
#define StencilMask stencil_mask                   // #define stencil_mask(phost, ...)           cmd(STENCIL_MASK           (__VA_ARGS__))
#define StencilOp stencil_op                       // #define stencil_op(phost, ...)             cmd(STENCIL_OP             (__VA_ARGS__))
#define Tag tag                                    // #define tag(phost, ...)                    cmd(TAG                    (__VA_ARGS__))
#define TagMask tag_mask                           // #define tag_mask(phost, ...)               cmd(TAG_MASK               (__VA_ARGS__))
#define Vertex2f1 vertex2f_1                       // #define vertex2f_16                 vertex2f_16
#define Vertex2f16 vertex2f_16                     // #define vertex2f_2                  vertex2f_2
#define Vertex2f2 vertex2f_2                       // #define vertex2f_4                  vertex2f_4
#define Vertex2f4 vertex2f_4                       // #define vertex2f_8                  vertex2f_8
#define Vertex2f8 vertex2f_8                       // #define vertex2f(phost, ...)               vertex2f_rewrite           (__VA_ARGS__)
#define Vertex2ii vertex2ii                        // #define vertex2ii(phost, ...)              cmd(VERTEX2II              (__VA_ARGS__))
#define Vertex2f vertex2f                          // #define vertex2f(phost, ...)               vertex2f_rewrite           (__VA_ARGS__)
#define VertexFormat vertex_format                 // #define vertex_format(phost, ...)          vertex_format_rewrite      (__VA_ARGS__)
#define VertexTranslateX vertex_translate_x        // #define vertex_translate_x(phost, ...)     cmd(VERTEX_TRANSLATE_X     (__VA_ARGS__))
#define VertexTranslateY vertex_translate_y        // #define vertex_translate_y(phost, ...)     cmd(VERTEX_TRANSLATE_Y     (__VA_ARGS__))

#define alpha_func(phost, ...) cmd(phost, ALPHA_FUNC(__VA_ARGS__))
#define begin(phost, ...) cmd(phost, BEGIN(__VA_ARGS__))
#define bitmap_addr_mask(phost, ...) cmd(phost, BITMAP_ADDR_MASK(__VA_ARGS__))
#define bitmap_source2(phost, ...) cmd(phost, BITMAP_SOURCE2(__VA_ARGS__))
#define bitmap_sourceh(phost, ...) cmd(phost, BITMAP_SOURCEH(__VA_ARGS__))
#define bitmap_transform_a_ex(phost, ...) cmd(phost, BITMAP_TRANSFORM_A_EX(__VA_ARGS__))
#define bitmap_transform_b_ex(phost, ...) cmd(phost, BITMAP_TRANSFORM_B_EX(__VA_ARGS__))
#define bitmap_transform_d_ex(phost, ...) cmd(phost, BITMAP_TRANSFORM_D_EX(__VA_ARGS__))
#define bitmap_transform_e_ex(phost, ...) cmd(phost, BITMAP_TRANSFORM_E_EX(__VA_ARGS__))
#define bitmap_zorder(phost, ...) cmd(phost, BITMAP_ZORDER(__VA_ARGS__))
#define bitmap_ext_format(phost, ...) cmd(phost, BITMAP_EXT_FORMAT(__VA_ARGS__))
#define bitmap_handle(phost, ...) cmd(phost, BITMAP_HANDLE(__VA_ARGS__))
#define bitmap_layout(phost, ...) cmd(phost, BITMAP_LAYOUT(__VA_ARGS__))
#define bitmap_layout_h(phost, ...) cmd(phost, BITMAP_LAYOUT_H(__VA_ARGS__))
#define bitmap_size(phost, ...) cmd(phost, BITMAP_SIZE(__VA_ARGS__))
#define bitmap_size_h(phost, ...) cmd(phost, BITMAP_SIZE_H(__VA_ARGS__))
#define bitmap_source(phost, ...) cmd(phost, BITMAP_SOURCE(__VA_ARGS__))
#define bitmap_source2(phost, ...) cmd(phost, BITMAP_SOURCE2(__VA_ARGS__))
#define bitmap_sourceh(phost, ...) cmd(phost, BITMAP_SOURCEH(__VA_ARGS__))
#define bitmap_swizzle(phost, ...) cmd(phost, BITMAP_SWIZZLE(__VA_ARGS__))
#define bitmap_transform_a(phost, ...) cmd(phost, BITMAP_TRANSFORM_A(__VA_ARGS__))
#define bitmap_transform_a_ext(phost, ...) cmd(phost, BITMAP_TRANSFORM_A_EXT(__VA_ARGS__))
#define bitmap_transform_b(phost, ...) cmd(phost, BITMAP_TRANSFORM_B(__VA_ARGS__))
#define bitmap_transform_b_ext(phost, ...) cmd(phost, BITMAP_TRANSFORM_B_EXT(__VA_ARGS__))
#define bitmap_transform_c(phost, ...) cmd(phost, BITMAP_TRANSFORM_C(__VA_ARGS__))
#define bitmap_transform_d(phost, ...) cmd(phost, BITMAP_TRANSFORM_D(__VA_ARGS__))
#define bitmap_transform_d_ext(phost, ...) cmd(phost, BITMAP_TRANSFORM_D_EXT(__VA_ARGS__))
#define bitmap_transform_e(phost, ...) cmd(phost, BITMAP_TRANSFORM_E(__VA_ARGS__))
#define bitmap_transform_e_ext(phost, ...) cmd(phost, BITMAP_TRANSFORM_E_EXT(__VA_ARGS__))
#define bitmap_transform_f(phost, ...) cmd(phost, BITMAP_TRANSFORM_F(__VA_ARGS__))
#define bitmap_zorder(phost, ...) cmd(phost, BITMAP_ZORDER(__VA_ARGS__))
#define blend_func(phost, ...) cmd(phost, BLEND_FUNC(__VA_ARGS__))
#define call(phost, ...) cmd(phost, CALL(__VA_ARGS__))
#define cell(phost, ...) cmd(phost, CELL(__VA_ARGS__))
#define clear(phost, ...) cmd(phost, CLEAR(__VA_ARGS__))
#define clear_color_a(phost, ...) cmd(phost, CLEAR_COLOR_A(__VA_ARGS__))
#define clear_color_rgb(phost, ...) cmd(phost, CLEAR_COLOR_RGB(__VA_ARGS__))
#define clear_stencil(phost, ...) cmd(phost, CLEAR_STENCIL(__VA_ARGS__))
#define clear_tag(phost, ...) cmd(phost, CLEAR_TAG(__VA_ARGS__))
#define color_a(phost, ...) cmd(phost, COLOR_A(__VA_ARGS__))
#define color_mask(phost, ...) cmd(phost, COLOR_MASK(__VA_ARGS__))
#define color_rgb(phost, ...) cmd(phost, COLOR_RGB(__VA_ARGS__))
#define display(phost, ...) cmd(phost, DISPLAY(__VA_ARGS__))
#define end(phost, ...) cmd(phost, END(__VA_ARGS__))
#define jump(phost, ...) cmd(phost, JUMP(__VA_ARGS__))
#define line_width(phost, ...) cmd(phost, LINE_WIDTH(__VA_ARGS__))
#define macro(phost, ...) cmd(phost, MACRO(__VA_ARGS__))
#define nop(phost, ...) cmd(phost, NOP(__VA_ARGS__))
#define palette_source(phost, ...) cmd(phost, PALETTE_SOURCE(__VA_ARGS__))
#define palette_sourceh(phost, ...) cmd(phost, PALETTE_SOURCEH(__VA_ARGS__))
#define palette_sourceh2(phost, ...) cmd(phost, PALETTE_SOURCEH2(__VA_ARGS__))
#define point_size(phost, ...) cmd(phost, POINT_SIZE(__VA_ARGS__))
#define restore_context(phost, ...) cmd(phost, RESTORE_CONTEXT())
#define return_(phost, ...) cmd(phost, RETURN(__VA_ARGS__))
#define save_context(phost, ...) cmd(phost, SAVE_CONTEXT())
#define scissor_size(phost, ...) cmd(phost, SCISSOR_SIZE(__VA_ARGS__))
#define scissor_xy(phost, ...) cmd(phost, SCISSOR_XY(__VA_ARGS__))
#define stencil_func(phost, ...) cmd(phost, STENCIL_FUNC(__VA_ARGS__))
#define stencil_mask(phost, ...) cmd(phost, STENCIL_MASK(__VA_ARGS__))
#define stencil_op(phost, ...) cmd(phost, STENCIL_OP(__VA_ARGS__))
#define tag(phost, ...) cmd(phost, TAG(__VA_ARGS__))
#define tag_mask(phost, ...) cmd(phost, TAG_MASK(__VA_ARGS__))
#define vertex2ii(phost, ...) cmd(phost, VERTEX2II(__VA_ARGS__))
#define vertex_translate_x(phost, ...) cmd(phost, VERTEX_TRANSLATE_X(__VA_ARGS__))
#define vertex_translate_y(phost, ...) cmd(phost, VERTEX_TRANSLATE_Y(__VA_ARGS__))

#define vertex_format(phost, ...) vertex_format_rewrite(phost, __VA_ARGS__)
#define vertex2f(phost, ...) vertex2f_rewrite(phost, __VA_ARGS__)
#define pointsize(phost, ...) pointsize_rewrite(phost, __VA_ARGS__)
extern void (*vertex2f_rewrite)(EVE_HalContext *phost, int32_t x, int32_t y);
void pointsize_rewrite(EVE_HalContext *phost, uint32_t size);
void vertex_format_rewrite(EVE_HalContext *phost, uint32_t format);
void vertex2f_1(EVE_HalContext *phost, int32_t x, int32_t y);
void vertex2f_2(EVE_HalContext *phost, int32_t x, int32_t y);
void vertex2f_4(EVE_HalContext *phost, int32_t x, int32_t y);
void vertex2f_8(EVE_HalContext *phost, int32_t x, int32_t y);
void vertex2f_16(EVE_HalContext *phost, int32_t x, int32_t y);

#define wr32(phost, ...) EVE_Hal_wr32(phost, __VA_ARGS__)
#define rd32(phost, ...) EVE_Hal_rd32(phost, __VA_ARGS__)
#define cmd(phost, ...) EVE_Cmd_wr32(phost, __VA_ARGS__)

#define cmd_animDraw(phost, ...) EVE_CoCmd_animDraw(phost, __VA_ARGS__)
#define cmd_animdraw(phost, ...) EVE_CoCmd_animDraw(phost, __VA_ARGS__)
#define cmd_animFrameRam(phost, ...) EVE_CoCmd_animFrameRam(phost, __VA_ARGS__)
#define cmd_animframeram(phost, ...) EVE_CoCmd_animFrameRam(phost, __VA_ARGS__)
#define cmd_animStartRam(phost, ...) EVE_CoCmd_animStartRam(phost, __VA_ARGS__)
#define cmd_animstartram(phost, ...) EVE_CoCmd_animStartRam(phost, __VA_ARGS__)
#define cmd_animStop(phost, ...) EVE_CoCmd_animStop(phost, __VA_ARGS__)
#define cmd_animstop(phost, ...) EVE_CoCmd_animStop(phost, __VA_ARGS__)
#define cmd_animXY(phost, ...) EVE_CoCmd_animXY(phost, __VA_ARGS__)
#define cmd_animxy(phost, ...) EVE_CoCmd_animXY(phost, __VA_ARGS__)
#define cmd_append(phost, ...) EVE_CoCmd_append(phost, __VA_ARGS__)
#define cmd_appendF(phost, ...) EVE_CoCmd_appendF(phost, __VA_ARGS__)
#define cmd_appendf(phost, ...) EVE_CoCmd_appendF(phost, __VA_ARGS__)
#define cmd_arc(phost, ...) EVE_CoCmd_arc(phost, __VA_ARGS__)
#define cmd_bgColor(phost, ...) EVE_CoCmd_bgColor(phost, __VA_ARGS__)
#define cmd_bgcolor(phost, ...) EVE_CoCmd_bgColor(phost, __VA_ARGS__)
#define cmd_bitmapTransform(phost, ...) EVE_CoCmd_bitmapTransform(phost, __VA_ARGS__)
#define cmd_bitmaptransform(phost, ...) EVE_CoCmd_bitmapTransform(phost, __VA_ARGS__)
#define cmd_button(phost, ...) EVE_CoCmd_button(phost, __VA_ARGS__)
#define cmd_calibrate(phost, ...) EVE_CoCmd_calibrate(phost, __VA_ARGS__)
#define cmd_calibrateSub(phost, ...) EVE_CoCmd_calibrateSub(phost, __VA_ARGS__)
#define cmd_calibratesub(phost, ...) EVE_CoCmd_calibrateSub(phost, __VA_ARGS__)
#define cmd_callList(phost, ...) EVE_CoCmd_callList(phost, __VA_ARGS__)
#define cmd_calllist(phost, ...) EVE_CoCmd_callList(phost, __VA_ARGS__)
#define cmd_cgradient(phost, ...) EVE_CoCmd_cgradient(phost, __VA_ARGS__)
#define cmd_clearCache(phost) EVE_CoCmd_clearCache(phost)
#define cmd_clearcache(phost) EVE_CoCmd_clearCache(phost)
#define cmd_clock(phost, ...) EVE_CoCmd_clock(phost, __VA_ARGS__)
#define cmd_coldStart(phost) EVE_CoCmd_coldStart(phost)
#define cmd_coldstart(phost) EVE_CoCmd_coldStart(phost)
#define cmd_dial(phost, ...) EVE_CoCmd_dial(phost, __VA_ARGS__)
#define cmd_dlStart(phost) EVE_CoCmd_dlStart(phost)
#define cmd_dlstart(phost) EVE_CoCmd_dlStart(phost)
#define cmd_endList(phost) EVE_CoCmd_endList(phost)
#define cmd_endlist(phost) EVE_CoCmd_endList(phost)
#define cmd_fence(phost, ...) EVE_CoCmd_fence(phost)
#define cmd_fgColor(phost, ...) EVE_CoCmd_fgColor(phost, __VA_ARGS__)
#define cmd_fgcolor(phost, ...) EVE_CoCmd_fgColor(phost, __VA_ARGS__)
#define cmd_fillWidth(phost, ...) EVE_CoCmd_fillWidth(phost, __VA_ARGS__)
#define cmd_fillwidth(phost, ...) EVE_CoCmd_fillWidth(phost, __VA_ARGS__)
#define cmd_flashAttach(phost) EVE_CoCmd_flashAttach(phost)
#define cmd_flashattach(phost) EVE_CoCmd_flashAttach(phost)
#define cmd_flashDetach(phost) EVE_CoCmd_flashDetach(phost)
#define cmd_flashdetach(phost) EVE_CoCmd_flashDetach(phost)
#define cmd_flashErase(phost) EVE_CoCmd_flashErase(phost)
#define cmd_flasherase(phost) EVE_CoCmd_flashErase(phost)
#define cmd_flashErase_flush(phost) EVE_CoCmd_flashErase_flush(phost)
#define cmd_flasherase_flush(phost) EVE_CoCmd_flashErase_flush(phost)
#define cmd_flashFast(phost, ...) EVE_CoCmd_flashFast(phost, __VA_ARGS__)
#define cmd_flashfast(phost, ...) EVE_CoCmd_flashFast(phost, __VA_ARGS__)
#define cmd_flashProgram(phost, ...) EVE_CoCmd_flashProgram(phost, __VA_ARGS__)
#define cmd_flashprogram(phost, ...) EVE_CoCmd_flashProgram(phost, __VA_ARGS__)
#define cmd_flashRead(phost, ...) EVE_CoCmd_flashRead(phost, __VA_ARGS__)
#define cmd_flashread(phost, ...) EVE_CoCmd_flashRead(phost, __VA_ARGS__)
#define cmd_flashRead_flush(phost, ...) EVE_CoCmd_flashRead_flush(phost, __VA_ARGS__)
#define cmd_flashread_flush(phost, ...) EVE_CoCmd_flashRead_flush(phost, __VA_ARGS__)
#define cmd_flashSource(phost, ...) EVE_CoCmd_flashSource(phost, __VA_ARGS__)
#define cmd_flashsource(phost, ...) EVE_CoCmd_flashSource(phost, __VA_ARGS__)
#define cmd_flashSpiDesel(phost) EVE_CoCmd_flashSpiDesel(phost)
#define cmd_flashspidesel(phost) EVE_CoCmd_flashSpiDesel(phost)
#define cmd_flashSpiRx(phost, ...) EVE_CoCmd_flashSpiRx(phost, __VA_ARGS__)
#define cmd_flashspirx(phost, ...) EVE_CoCmd_flashSpiRx(phost, __VA_ARGS__)
#define cmd_flashSpiTx(phost, ...) EVE_CoCmd_flashSpiTx(phost, __VA_ARGS__)
#define cmd_flashspitx(phost, ...) EVE_CoCmd_flashSpiTx(phost, __VA_ARGS__)
#define cmd_flashUpdate(phost, ...) EVE_CoCmd_flashUpdate(phost, __VA_ARGS__)
#define cmd_flashupdate(phost, ...) EVE_CoCmd_flashUpdate(phost, __VA_ARGS__)
#define cmd_flashWrite(phost, ...) EVE_CoCmd_flashWrite(phost, __VA_ARGS__)
#define cmd_flashwrite(phost, ...) EVE_CoCmd_flashWrite(phost, __VA_ARGS__)
#define cmd_fsfile(phost, ...) EVE_CoCmd_fsfile(phost, __VA_ARGS__)
#define cmd_fswrite(phost, ...) EVE_CoCmd_fswrite(phost, __VA_ARGS__)
#define cmd_fsread(phost, ...) EVE_CoCmd_fsread(phost, __VA_ARGS__)
#define cmd_fssize(phost, ...) EVE_CoCmd_fssize(phost, __VA_ARGS__)
#define cmd_fssource(phost, ...) EVE_CoCmd_fssource(phost, __VA_ARGS__)
#define cmd_gauge(phost, ...) EVE_CoCmd_gauge(phost, __VA_ARGS__)
#define cmd_getImage(phost, ...) EVE_CoCmd_getImage(phost, __VA_ARGS__)
#define cmd_getimage(phost, ...) EVE_CoCmd_getImage(phost, __VA_ARGS__)
#define cmd_getMatrix(phost, ...) EVE_CoCmd_getMatrix(phost, __VA_ARGS__)
#define cmd_getmatrix(phost, ...) EVE_CoCmd_getMatrix(phost, __VA_ARGS__)
#define cmd_getProps(phost, ...) EVE_CoCmd_getProps(phost, __VA_ARGS__)
#define cmd_getprops(phost, ...) EVE_CoCmd_getProps(phost, __VA_ARGS__)
#define cmd_getPtr(phost, ...) EVE_CoCmd_getPtr(phost, __VA_ARGS__)
#define cmd_getptr(phost, ...) EVE_CoCmd_getPtr(phost, __VA_ARGS__)
#define cmd_glow(phost, ...) EVE_CoCmd_glow(phost, __VA_ARGS__)
#define cmd_gradColor(phost, ...) EVE_CoCmd_gradColor(phost, __VA_ARGS__)
#define cmd_gradcolor(phost, ...) EVE_CoCmd_gradColor(phost, __VA_ARGS__)
#define cmd_gradient(phost, ...) EVE_CoCmd_gradient(phost, __VA_ARGS__)
#define cmd_gradientA(phost, ...) EVE_CoCmd_gradientA(phost, __VA_ARGS__)
#define cmd_gradienta(phost, ...) EVE_CoCmd_gradientA(phost, __VA_ARGS__)
#define cmd_inflate(phost, ...) EVE_CoCmd_inflate(phost, __VA_ARGS__)
#define cmd_inflate_flash(phost, ...) EVE_CoCmd_inflate_flash(phost, __VA_ARGS__)
#define cmd_inflate_progMem(phost, ...) EVE_CoCmd_inflate_progMem(phost, __VA_ARGS__)
#define cmd_inflate_progmem(phost, ...) EVE_CoCmd_inflate_progMem(phost, __VA_ARGS__)
#define cmd_interrupt(phost, ...) EVE_CoCmd_interrupt(phost, __VA_ARGS__)
#define cmd_keys(phost, ...) EVE_CoCmd_keys(phost, __VA_ARGS__)
#define cmd_loadasset(phost, ...) EVE_CoCmd_loadasset(phost, __VA_ARGS__)
#define cmd_loadIdentity(phost) EVE_CoCmd_loadIdentity(phost)
#define cmd_loadidentity(phost) EVE_CoCmd_loadIdentity(phost)
#define cmd_loadImage(phost, ...) EVE_CoCmd_loadImage(phost, __VA_ARGS__)
#define cmd_loadimage(phost, ...) EVE_CoCmd_loadImage(phost, __VA_ARGS__)
#define cmd_loadImage_flash(phost, ...) EVE_CoCmd_loadImage_flash(phost, __VA_ARGS__)
#define cmd_loadimage_flash(phost, ...) EVE_CoCmd_loadImage_flash(phost, __VA_ARGS__)
#define cmd_loadImage_progMem(phost, ...) EVE_CoCmd_loadImage_progMem(phost, __VA_ARGS__)
#define cmd_loadimage_progmem(phost, ...) EVE_CoCmd_loadImage_progMem(phost, __VA_ARGS__)
#define cmd_loadpatch(phost, ...) EVE_CoCmd_loadPatch(phost, __VA_ARGS__)
#define cmd_messagebox(phost, ...) EVE_CoCmd_messageBox(phost, __VA_ARGS__)
#define cmd_logo(phost) EVE_CoCmd_logo(phost)
#define cmd_mediaFifo(phost, ...) EVE_CoCmd_mediaFifo(phost, __VA_ARGS__)
#define cmd_mediafifo(phost, ...) EVE_CoCmd_mediaFifo(phost, __VA_ARGS__)
#define cmd_memCpy(phost, ...) EVE_CoCmd_memCpy(phost, __VA_ARGS__)
#define cmd_memcpy(phost, ...) EVE_CoCmd_memCpy(phost, __VA_ARGS__)
#define cmd_memCrc(phost, ...) EVE_CoCmd_memCrc(phost, __VA_ARGS__)
#define cmd_memcrc(phost, ...) EVE_CoCmd_memCrc(phost, __VA_ARGS__)
#define cmd_memSet(phost, ...) EVE_CoCmd_memSet(phost, __VA_ARGS__)
#define cmd_memset(phost, ...) EVE_CoCmd_memSet(phost, __VA_ARGS__)
#define cmd_memWrite(phost, ...) EVE_CoCmd_memWrite(phost, __VA_ARGS__)
#define cmd_memwrite(phost, ...) EVE_CoCmd_memWrite(phost, __VA_ARGS__)
#define cmd_memWrite32(phost, ...) EVE_CoCmd_memWrite32(phost, __VA_ARGS__)
#define cmd_memwrite32(phost, ...) EVE_CoCmd_memWrite32(phost, __VA_ARGS__)
#define cmd_memZero(phost, ...) EVE_CoCmd_memZero(phost, __VA_ARGS__)
#define cmd_memzero(phost, ...) EVE_CoCmd_memZero(phost, __VA_ARGS__)
#define cmd_newList(phost, ...) EVE_CoCmd_newList(phost, __VA_ARGS__)
#define cmd_newlist(phost, ...) EVE_CoCmd_newList(phost, __VA_ARGS__)
#define cmd_nop(phost) EVE_CoCmd_nop(phost)
#define cmd_number(phost, ...) EVE_CoCmd_number(phost, __VA_ARGS__)
#define cmd_playVideo(phost, ...) EVE_CoCmd_playVideo(phost, __VA_ARGS__)
#define cmd_playvideo(phost, ...) EVE_CoCmd_playVideo(phost, __VA_ARGS__)
#define cmd_progress(phost, ...) EVE_CoCmd_progress(phost, __VA_ARGS__)
#define cmd_regRead(phost, ...) EVE_CoCmd_regRead(phost, __VA_ARGS__)
#define cmd_regread(phost, ...) EVE_CoCmd_regRead(phost, __VA_ARGS__)
#define cmd_renderTarget(phost, ...) EVE_CoCmd_renderTarget(phost, __VA_ARGS__)
#define cmd_rendertarget(phost, ...) EVE_CoCmd_renderTarget(phost, __VA_ARGS__)
#define cmd_resetFonts(phost) EVE_CoCmd_resetFonts(phost)
#define cmd_resetfonts(phost) EVE_CoCmd_resetFonts(phost)
#define cmd_return(phost) EVE_CoCmd_return(phost)
#define cmd_romFont(phost, ...) EVE_CoCmd_romFont(phost, __VA_ARGS__)
#define cmd_romfont(phost, ...) EVE_CoCmd_romFont(phost, __VA_ARGS__)
#define cmd_rotate(phost, ...) EVE_CoCmd_rotate(phost, __VA_ARGS__)
#define cmd_rotateAround(phost, ...) EVE_CoCmd_rotateAround(phost, __VA_ARGS__)
#define cmd_rotatearound(phost, ...) EVE_CoCmd_rotateAround(phost, __VA_ARGS__)
#define cmd_runAnim(phost, ...) EVE_CoCmd_runAnim(phost, __VA_ARGS__)
#define cmd_runanim(phost, ...) EVE_CoCmd_runAnim(phost, __VA_ARGS__)
#define cmd_scale(phost, ...) EVE_CoCmd_scale(phost, __VA_ARGS__)
#define cmd_screenSaver(phost) EVE_CoCmd_screenSaver(phost)
#define cmd_screensaver(phost) EVE_CoCmd_screenSaver(phost)
#define cmd_scrollbar(phost, ...) EVE_CoCmd_scrollbar(phost, __VA_ARGS__)
#define cmd_sdattach(phost, ...) EVE_CoCmd_sdattach(phost, __VA_ARGS__)
#define cmd_setBase(phost, ...) EVE_CoCmd_setBase(phost, __VA_ARGS__)
#define cmd_setbase(phost, ...) EVE_CoCmd_setBase(phost, __VA_ARGS__)
#define cmd_setBitmap(phost, ...) EVE_CoCmd_setBitmap(phost, __VA_ARGS__)
#define cmd_setbitmap(phost, ...) EVE_CoCmd_setBitmap(phost, __VA_ARGS__)
#define cmd_setFont(phost, ...) EVE_CoCmd_setFont(phost, __VA_ARGS__)
#define cmd_setfont(phost, ...) EVE_CoCmd_setFont(phost, __VA_ARGS__)
#define cmd_setMatrix(phost) EVE_CoCmd_setMatrix(phost)
#define cmd_setmatrix(phost) EVE_CoCmd_setMatrix(phost)
#define cmd_setRotate(phost, ...) EVE_CoCmd_setRotate(phost, __VA_ARGS__)
#define cmd_setrotate(phost, ...) EVE_CoCmd_setRotate(phost, __VA_ARGS__)
#define cmd_setScratch(phost, ...) EVE_CoCmd_setScratch(phost, __VA_ARGS__)
#define cmd_setscratch(phost, ...) EVE_CoCmd_setScratch(phost, __VA_ARGS__)
#define cmd_sketch(phost, ...) EVE_CoCmd_sketch(phost, __VA_ARGS__)
#define cmd_slider(phost, ...) EVE_CoCmd_slider(phost, __VA_ARGS__)
#define cmd_snapshot(phost, ...) EVE_CoCmd_snapshot(phost, __VA_ARGS__)
#define cmd_spinner(phost, ...) EVE_CoCmd_spinner(phost, __VA_ARGS__)
#define cmd_stop(phost) EVE_CoCmd_stop(phost)
#define cmd_swap(phost) EVE_CoCmd_swap(phost)
#define cmd_sync(phost) EVE_CoCmd_sync(phost)
#define cmd_testCard(phost) EVE_CoCmd_testCard(phost)
#define cmd_testcard(phost) EVE_CoCmd_testCard(phost)
#define cmd_text(phost, ...) EVE_CoCmd_text(phost, __VA_ARGS__)
#define cmd_text_ex(phost, ...) EVE_CoCmd_text_ex(phost, __VA_ARGS__)
#define cmd_text_s(phost, ...) EVE_CoCmd_text_s(phost, __VA_ARGS__)
#define cmd_toggle(phost, ...) EVE_CoCmd_toggle(phost, __VA_ARGS__)
#define cmd_track(phost, ...) EVE_CoCmd_track(phost, __VA_ARGS__)
#define cmd_translate(phost, ...) EVE_CoCmd_translate(phost, __VA_ARGS__)
#define cmd_videoFrame(phost, ...) EVE_CoCmd_videoFrame(phost, __VA_ARGS__)
#define cmd_videoframe(phost, ...) EVE_CoCmd_videoFrame(phost, __VA_ARGS__)
#define cmd_videoStart(phost, ...) EVE_CoCmd_videoStart(phost, __VA_ARGS__)
#define cmd_videostart(phost, ...) EVE_CoCmd_videoStart(phost, __VA_ARGS__)
#define cmd_wait(phost, ...) EVE_CoCmd_wait(phost, __VA_ARGS__)
#define cmd_watchdog(phost, ...) EVE_CoCmd_watchdog(phost, __VA_ARGS__)
#define cmd_graphicsfinish(phost, ...) EVE_CoCmd_graphicsfinish(phost)

#endif /* EW2024_PHOTOBOOTH_UTILS_H_ */
