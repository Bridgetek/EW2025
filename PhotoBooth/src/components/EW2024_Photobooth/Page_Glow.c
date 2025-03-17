/**
 * @file Page_Glow.c
 * @brief A simple page showing a glowing circle
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth.h"
#include "EW2024_Photobooth_Utils.h"

static int event()
{
    return PAGE_CONTINUE;
}
static int drawing()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    uint32_t touch_screen_x = ges->touchX;
    uint32_t touch_screen_y = ges->touchY;
    uint32_t GLOW_W = 600;
    uint32_t GLOW_H = 400;

    if (!ges->isTouch)
        return;

    ColorRGB(PHOST, 0xa5, 0xba, 0x2a);
    cmd_glow(PHOST, touch_screen_x - GLOW_W / 2, touch_screen_y - GLOW_H / 2, GLOW_W, GLOW_H);
    ColorRGB(PHOST, 0xff, 0xff, 0xff);
}
void page_glow_init() {}
void page_glow_deinit() {}
int page_glow_draw()
{
    int ret = event();
    drawing();
    return ret;
}

int page_glow_load()
{
}