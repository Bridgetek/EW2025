/**
 * @file Page_Camera_Effect.c
 * @brief Page handling camera effects
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h> // Needed for sin(), pow()

#include "EW2024_Photobooth.h"
#include "EW2024_Photobooth_Utils.h"

// Identity function: l_ident
int l_ident(int p)
{
    return p;
}

// Darken function: l_darken
int l_darken(int p)
{
    return (int)(255 * pow(p / 255.0, 2.2));
}

// Brighten function: l_brighten
int l_brighten(int p)
{
    return (int)(255 * pow(p / 255.0, 0.5));
}

// Invert function: l_invert
int l_invert(int p)
{
    return 255 - p;
}

// Threshold function: l_thresh
int l_thresh(int p)
{
    return 255 * (p > 128);
}

// Contrast function: l_contrast
int l_contrast(int p)
{
    return (int)((p - 128) * 1.5 + 128);
}

// Reduce contrast function: l_reduce_contrast
int l_reduce_contrast(int p)
{
    return (int)((p - 128) * 0.5 + 128);
}

// Solarize function: l_solarize
int l_solarize(int p)
{
    return (p < 128) ? p : 255 - p;
}

// Posterize function: l_posterize
int l_posterize(int p)
{
    return (p / 64) * 64;
}

// HDR function: l_hdr
int l_hdr(int p)
{
    if (p > 128)
    {
        return (int)(255 * pow(p / 255.0, 0.7));
    }
    else
    {
        return (int)(255 * pow(p / 255.0, 1.3));
    }
}

// Advanced HDR function: l_hdr_advanced
int l_hdr_advanced(int p)
{
    return (int)(255 * (sin((p / 255.0) * M_PI - M_PI / 2) + 1) / 2);
}

// Combined HDR function: l_hdr_combined
int l_hdr_combined(int p)
{
    double val = (p / 255.0 - 0.5) * 2 + 0.5;
    if (val < 0)
        val = 0;
    if (val > 1)
        val = 1;
    return (int)(255 * val);
}

typedef struct
{
    int (*p)(int);
    char name[100];
    int32_t lut[256];
    uint32_t ramg_lut;
} trials_t;

trials_t trials[] = {
    {l_ident, "Normal", {0}, {0}},
    {l_darken, "Darken", {0}, {0}},
    {l_brighten, "Brighten", {0}, {0}},
    {l_invert, "Invert", {0}, {0}},
    {l_thresh, "Thresh", {0}, {0}},
    {l_contrast, "Contrast+", {0}, {0}},
    {l_reduce_contrast, "Contrast-", {0}, {0}},
    {l_solarize, "Solarize", {0}, {0}},
    {l_posterize, "Posterize", {0}, {0}},
    {l_hdr, "Pseudo-HDR", {0}, {0}},
    {l_hdr_advanced, "Advanced Pseudo-HDR", {0}, {0}},
    {l_hdr_combined, "Combined HDR", {0}, {0}},
};

#define trials_count sizeof(trials) / sizeof(trials[0])
int32_t pro_mode = 0;

// depends on the page_camera.c
extern uint32_t lvds_tag;

extern int32_t lvdsrx_w;
extern int32_t lvdsrx_h;

extern uint32_t lvds_input_addr; // Data address pull from EVE
extern uint32_t ramg_render_buffer_addr;
static uint32_t btn_effect_tag = TAG_INC;

extern uint32_t taskbar_icon_x;
extern uint32_t taskbar_icon_y;

static uint32_t picture_w = 1920;
static uint32_t picture_h = 1080;

static uint32_t minimap_scale = 40;
static box minimap = {0, 0, 1920 / 4, 1080 / 4};
static box minimap_shadow = {0, 0, 1920 / 4, 1080 / 4};
static uint32_t tag_minimap = 0;
static uint32_t lvdsrx_pr_addr = 0;
static bool moving = 1, expand = 0;

static void page_camera_effect_init_one()
{
    static bool is_page_camera_effect_init = 0;
    if (is_page_camera_effect_init == 1)
        return;
    is_page_camera_effect_init = 1;

    btn_effect_tag = utils_tagAlloc(TAG_INC);
    tag_minimap = utils_tagAlloc(TAG_INC);

    lvdsrx_pr_addr = utils_ddrAllocAlignment(2048 * 2048 * 2, 128);

    for (int i = 0; i < trials_count; i++)
    {
        trials_t *trial = &trials[i];
        for (int j = 0; j < 256; j++)
        {
            int value = (int)(trial->p(j)) * 0x010000;
            // Ensure value fits within 4 bytes
            if (value < 0)
            {
                value = 0;
            }
            else if (value > 0xFFFFFFFF)
            {
                value = 0xFFFFFFFF;
            }
            trial->lut[j] = value;
        }
        trial->ramg_lut = utils_ddrAllocAlignment(1024, 64);
        EVE_Hal_wrMem(PHOST, trial->ramg_lut, (char *)trial->lut, 1024);
        finish(PHOST);
    }
}

static void page_camera_effect_deinit_one()
{
    static bool is_page_camera_effect_deinit = 0;
    if (is_page_camera_effect_deinit == 1)
        return;
    is_page_camera_effect_deinit = 1;
}

static int event_on_minimap_scaling()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    if (ges->tagPressed == tag_minimap)
    {
        moving = 1;
        expand = 0;
        if (minimap.w != minimap_shadow.w)
        {
            // expand mode
            expand = 1;
        }
        else if (minimap.x != minimap_shadow.x || minimap.y != minimap_shadow.y)
        {
            // moving mode
            moving = 1;
        }
        // touch at left side switch to moving mode
        if (abs(ges->touchX - minimap.x) < abs(ges->touchX - minimap.x - minimap.w))
        {
            // moving mode
            moving = 1;
        }
        else
        {
            // expand mode
            expand = 1;
        }

        if (expand)
        {
            minimap.w = max(min(lvdsrx_w * 3 / 2, minimap_shadow.w + ges->distance), 50);
            minimap_scale = min(80, minimap.w * 100 / lvdsrx_w);

            // gap adjustment
            minimap.w = lvdsrx_w * minimap_scale / 100;
            minimap.h = lvdsrx_h * minimap_scale / 100;
        }
        else if (moving)
        {
            minimap.x = max(0, min(1900, minimap_shadow.x + ges->distanceX));
            minimap.y = max(0, min(1900, minimap_shadow.y + ges->distanceY));
        }
    }
    else
    {
        minimap_shadow.w = minimap.w;
        minimap_shadow.x = minimap.x;
        minimap_shadow.y = minimap.y;
    }
    return PAGE_CONTINUE;
}
static int event()
{
    event_on_minimap_scaling();

    Gesture_Touch_t *ges = utils_gestureGet();
    if (ges->tagPressed1 == btn_effect_tag)
    {
        pro_mode += 1;
        pro_mode = (pro_mode) % (trials_count);
    }
    return PAGE_CONTINUE;
}

static void draw_menu()
{
    int offset_x = taskbar_icon_x;
    int offset_y = taskbar_icon_y;
    int w = 350;
    int h = 70;
    int x = PHOST->Width / 2 - w / 2;
    ;
    int y = 1080 + (120 / 2 - h / 2);
    int font = 29;
    uint16_t options = 0;

    tag(PHOST, btn_effect_tag);
    EVE_CoCmd_fgColor(PHOST, 0xFF8C1A);
    EVE_CoCmd_button(PHOST, x, y, w, h, font, options, trials[pro_mode].name);

    // a bigger touch transprent area
    color_a(PHOST, 0);
    tag(PHOST, btn_effect_tag);
    utils_drawBoxText_4(PHOST, x - 10, y - 10, w + 40, h + 40);
    color_a(PHOST, 255);
}

static int camera_effect()
{
    int32_t pic_mode = 0;
    int32_t bitmap_bytes = 3038; // int(math.ceil(src.w * src.h * (bpp(src.fmt) // 8) / 0x800))
    uint32_t lut_address = trials[pro_mode].ramg_lut;

    // Prepare the LUT table, size = stridexh, h=2048
    for (int y = 0; y < bitmap_bytes; y += 2048)
    {
        int o = y * 2048;
        int h1 = min(bitmap_bytes - y, 2048);

        cmd_rendertarget(PHOST, lvdsrx_pr_addr + o, L8, 2048, h1);
        cmd_setbitmap(PHOST, lvds_input_addr + o, PALETTEDARGB8, 2048, h1);
        PaletteSource(PHOST, lut_address);
        EVE_Cmd_wr32(PHOST, PALETTE_SOURCEH2(lut_address));

        BlendFunc(PHOST, ONE, ZERO);
        Begin(PHOST, BITMAPS);
        Vertex2f(PHOST, 0, 0);

        utils_displayEnd(PHOST);
        utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
        vertex_format(PHOST, 1);
    }

    // draw camera with effect tag 0
    Tag(PHOST, 0);
    Begin(PHOST, BITMAPS);
    cmd_setbitmap(PHOST, lvdsrx_pr_addr, RGB8, 1920, 1080);
    Vertex2f(PHOST, 0, 0);

    cmd_rendertarget(PHOST, ramg_render_buffer_addr, RGB8, 1920, 1200);
}

/**
 * @brief Draw a minimap of the current image
 *
 * This function draws a red box around the minimap, and draws the minimap
 * itself.  The minimap is drawn at the position given by the values in the
 * minimap struct, and is scaled to the size given by the value in the
 * minimap_scale variable.
 *
 * @return 0 on success, or -1 on failure
 */
static int drawing_minimap()
{
    int border = 6;

    // border
    utils_drawBoxText(PHOST, minimap.x, minimap.y, minimap.w + border, minimap.h + border, 0, (char[]){0, 0, 255}, tag_minimap, "", 30, 0, (char[]){255, 255, 255});

    // the map
    Tag(PHOST, tag_minimap);
    cmd_setbitmap(PHOST, lvds_input_addr, RGB8, lvdsrx_w, lvdsrx_h);
    utils_scale(minimap_scale);
    Begin(PHOST, BITMAPS);
    Vertex2f(PHOST, minimap.x + border / 2, minimap.y + border / 2);
    utils_scale(100);

    // control area
    if (utils_gestureGet()->tagPressed == tag_minimap)
    {
        color_a(PHOST, 50);

        Tag(PHOST, tag_minimap);
        utils_drawRect(minimap.x + border, minimap.y + border, minimap.w / 2 - border * 2, minimap.h - border * 2);
        Image *img = IMG_ICON_MOVE;
        int imx = minimap.x + border + minimap.w / 4 - img->w / 2;
        int imy = minimap.y + border + minimap.h / 2 - img->h / 2;
        if (moving)
            color_a(PHOST, 255);
        utils_drawImageXyTag(PHOST, img, imx, imy, tag_minimap);
        if (moving)
            color_a(PHOST, 50);

        img = IMG_ICON_EXPAND;
        Tag(PHOST, tag_minimap);
        utils_drawRect(minimap.x + border + minimap.w / 2 + 2, minimap.y + border, minimap.w / 2 - border * 2 - 2, minimap.h - border * 2);
        imx += minimap.w / 2;
        if (expand)
            color_a(PHOST, 255);
        utils_drawImageXyTag(PHOST, img, imx, imy, tag_minimap);
    }
    color_a(PHOST, 255);
}
static int drawing_taskbar()
{
    utils_drawImageXY(PHOST, IMG_TASKBAR, 0, 1080);
}
static int drawing()
{
    camera_effect();
    drawing_minimap();

    utils_scale(100);
    drawing_taskbar();
    draw_menu();

    return 0;
}
void page_camera_effect_init()
{
    page_camera_effect_init_one();

    minimap.x = 0;
    minimap.y = 0;
    minimap.w = 1920 / 4;
    minimap.h = 1080 / 4;
    minimap_shadow = minimap;

    minimap_scale = minimap.w * 100 / lvdsrx_w;

    // gap adjustment
    minimap.w = lvdsrx_w * minimap_scale / 100;
    minimap.h = lvdsrx_h * minimap_scale / 100;
}
void page_camera_effect_deinit()
{
    page_camera_effect_deinit_one();
}
int page_camera_effect_draw()
{
    int ret = event();
    drawing();
    return ret;
}
int page_camera_effect_load()
{
}
