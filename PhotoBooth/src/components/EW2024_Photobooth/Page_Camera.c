/**
 * @file Page_Camera.c
 * @brief Camera page
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth.h"

uint32_t lvds_bitmap_handler = BMHL_AUTO;
uint32_t lvds_tag = TAG_INC;

const int32_t lvdsrx_w = 1920;
const int32_t lvdsrx_h = 1080;

extern uint32_t lvds_input_addr; // Data address pull from EVE

box lvdsrx_map = {0, 0, 1920, 1080};
int32_t lvdsrx_scale = 100;

static void page_camera_init_one()
{
    static bool is_page_camera_init = 0;
    if (is_page_camera_init == 1)
    {
        return;
    }
    is_page_camera_init = 1;

    // LVDS bitmap setup
    lvds_bitmap_handler = utils_BitmapHandler_get();
    lvds_tag = utils_tagAlloc(lvds_tag);
    Begin(PHOST, BITMAPS);
    bitmap_handle_set(PHOST, lvds_bitmap_handler);
    cmd_setbitmap(PHOST, lvds_input_addr, RGB8, lvdsrx_w, lvdsrx_h);
}
static void page_camera_deinit_one()
{
    static bool is_page_camera_deinit = 0;
    if (is_page_camera_deinit == 1)
    {
        return;
    }
    is_page_camera_deinit = 1;
}
static int event()
{
    return PAGE_CONTINUE;
}

static int drawing()
{
    if (lvdsrx_scale != 100)
    {
        cmd_loadidentity(PHOST);
        EVE_CoCmd_scale(PHOST, 65536 * lvdsrx_scale / 100, 65536 * lvdsrx_scale / 100);
    }

    Tag(PHOST, lvds_tag);
    cmd_setbitmap(PHOST, lvds_input_addr, RGB8, lvdsrx_w, lvdsrx_h);
    Begin(PHOST, BITMAPS);
    if (lvdsrx_scale != 100)
    {
        int32_t frameW = lvdsrx_w * lvdsrx_scale / 100;
        int32_t frameH = lvdsrx_h * lvdsrx_scale / 100;
        bitmap_size(PHOST, NEAREST, BORDER, BORDER, 0, 0);
        bitmap_size_h(PHOST, 0, 0);
        cmd_translate(PHOST, lvdsrx_map.x * 65536, lvdsrx_map.y * 65536);
        cmd_setmatrix(PHOST);
    }
    Vertex2f(PHOST, 0, 0);

    if (lvdsrx_scale != 100)
    {
        utils_scale(100);
    }

    utils_drawImageXY(PHOST, IMG_TASKBAR, 0, 1080);

    return 0;
}
void page_camera_init()
{
    page_camera_init_one();
}
void page_camera_deinit()
{
    page_camera_deinit_one();
}
int page_camera_draw()
{
    int ret = event();
    drawing();
    return ret;
}
int page_camera_load()
{
    utils_scanoutInitDefault(PHOST);
    utils_scanoutQueryPtr();
}