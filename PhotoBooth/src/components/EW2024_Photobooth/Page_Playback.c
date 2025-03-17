/**
 * @file Page_Playback.c
 * @brief Sticker page
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth.h"
#include "EW2024_Photobooth_Utils.h"

static const char *avi = "assets_sdcard_eve/photobooth2024/avi/istock.avi";
static uint32_t video_bitmap_handler = BMHL_AUTO;
static uint32_t glow_x = 0, glow_y = 0, glow_on = 0;
uint32_t video_addr = 0;
uint32_t video_completion_ptr = 0;
uint32_t video_w = 1920;
uint32_t video_h = 1200;

static int event()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    glow_on = 0;

    if (ges->isTouch)
    {
        glow_x = ges->touchX;
        glow_y = ges->touchY;
        glow_on = 1;
    }
    return PAGE_CONTINUE;
}

static int drawing()
{
    // end the display list
    EVE_CoDl_display(PHOST);
    EVE_CoCmd_swap(PHOST);
    EVE_CoCmd_graphicsfinish(PHOST);
    EVE_Cmd_waitFlush(PHOST);

    cmd_romfont(PHOST, 32, 34);
    cmd_fssource(PHOST, avi, 0);
    finish(PHOST);

    cmd_videostart(PHOST, OPT_FS | OPT_YCBCR);
    cmd_videoframe(PHOST, video_addr, video_completion_ptr);
    finish(PHOST);

    // first frame
    utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
    BitmapHandle(PHOST, video_bitmap_handler);
    Begin(PHOST, BITMAPS);
    cmd_setbitmap(PHOST, video_addr, YCBCR, video_w, video_h);
    vertex2f(PHOST, 0, 0);
    EVE_CoDl_display(PHOST);
    EVE_CoCmd_swap(PHOST);
    EVE_CoCmd_graphicsfinish(PHOST);
    EVE_Cmd_waitFlush(PHOST);

    const uint32_t GLOW_W = 600;
    const uint32_t GLOW_H = 400;

    while (EVE_Hal_rd32(PHOST, video_completion_ptr) != 0)
    {
        Gesture_Touch_t *ges = utils_gestureRenew(PHOST);

        if (ges->tagReleased == IMG_BTN_BACK->tagval)
        {
            // restore display list before end video
            utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
            page_playback.isActive = 0;
            return 0;
        }
        event();

        utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
        vertex_format(PHOST, 1);

        cmd_videoframe(PHOST, video_addr, video_completion_ptr);
        finish(PHOST);

        BitmapHandle(PHOST, video_bitmap_handler);
        Begin(PHOST, BITMAPS);
        vertex2f(PHOST, 0, 0);

        if (glow_on)
        {
            SaveContext(PHOST);
            ColorRGB(PHOST, 0xa5, 0xba, 0x2a);
            cmd_glow(PHOST, max(0, glow_x - GLOW_W / 2), max(0, glow_y - GLOW_H / 2), GLOW_W, GLOW_H);
            RestoreContext(PHOST);
        }

        {
            Image *img = IMG_BTN_BACK;
            uint32_t x = PHOST->Width / 2 - img->w, y = PHOST->Height - img->h - 10;
            Tag(PHOST, img->tagval);
            utils_drawImageXY(PHOST, img, x, y);
        }

        EVE_CoDl_display(PHOST);
        EVE_CoCmd_swap(PHOST);
        EVE_CoCmd_graphicsfinish(PHOST);
        EVE_Cmd_waitFlush(PHOST);
    }

    return 0;
}

static void page_playback_init_one()
{
    static bool is_page_playback_init = 0;
    if (is_page_playback_init == 1)
        return;
    is_page_playback_init = 1;

    video_bitmap_handler = utils_BitmapHandler_get();
    IMG_BTN_BACK->tagval = utils_tagAlloc(TAG_INC);

    video_addr = utils_ddrAlloc(video_w * video_h * 2);
    video_completion_ptr = utils_ddrAlloc(4);
}

static void page_playback_deinit_one()
{
    static bool is_page_playback_deinit = 0;
    if (is_page_playback_deinit == 1)
        return;
    is_page_playback_deinit = 1;
}

void page_playback_load()
{
}

int page_playback_draw()
{
    int ret = event();
    drawing();
    return ret;
}

void page_playback_init()
{
    page_playback_init_one();
}

void page_playback_deinit()
{
    page_playback_deinit_one();
}
