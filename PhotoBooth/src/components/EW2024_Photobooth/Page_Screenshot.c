/**
 * @file Page_Screenshot.c
 * @brief Take a screenshot and review it
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
extern int32_t lvdsrx_w;
extern int32_t lvdsrx_h;

static uint32_t tag_border_start = TAG_INC;
static uint32_t tag_border_end = TAG_INC;
static uint32_t tag_border_tl = TAG_INC; // top left
static uint32_t tag_border_tr = TAG_INC; // top right
static uint32_t tag_border_bl = TAG_INC; // bottom left
static uint32_t tag_border_br = TAG_INC; // bottom right
static uint32_t tag_border_l = TAG_INC;
static uint32_t tag_border_r = TAG_INC;
static uint32_t tag_border_t = TAG_INC;
static uint32_t tag_border_b = TAG_INC;
static uint32_t tag_edge_selected = 0, tag_edge_moving = 0;

static char color_unselected[] = {0xB8, 0xB8, 0xB8};
static char color_selected[] = {0XE6, 0xF2, 0xFF};
static char *border_color = color_unselected;

static uint32_t minimap_scale = 100;
static box minimap = {0, 0, 1920, 1080};
static box minimap_shadow = {0, 0, 1920, 1080};
static uint32_t tag_camera = 0;
extern uint32_t lvds_input_addr; // Data address pull from EVE

extern uint32_t taskbar_icon_x;
extern uint32_t taskbar_icon_y;

#define CAM_MINX (0)
#define CAM_MINY (0)
#define CAM_MINW (1920 * 20 / 100)
#define CAM_MINH (1200 * 40 / 100)

static uint32_t tag_btn_save_2_sdcard = 0;
static uint32_t tag_btn_capture = 0;
static uint32_t tag_btn_buffer1 = 0;
static uint32_t tag_btn_buffer2 = 0;
static uint32_t tag_btn_buffer3 = 0;
static uint32_t buffer_nth_selected = 0; // 0 to 2

static bool moving = 0, expand = 0;

static bool dynamic_point = 1;
extern uint32_t ramg_render_buffer_addr;
extern uint32_t lvds_input_addr; // Data address pull from EVE

uint32_t ramg_render_screenshot_addr;

typedef struct
{
    int x, y, w, h;
    uint32_t ptr;
    bool initialized, moved_done;
    int wzoom, hzoom;
    int zoom;
    int xdest, ydest, wdest, hdest;
} buffer_onscreen_t;

#define capture_w lvdsrx_w / 10
#define capture_h lvdsrx_h / 10

buffer_onscreen_t buffer_onscreen[3];
int buffer_viewing = -1;

static int event_on_camera_scaling()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    tag_edge_selected = 0;

    if (moving && ges->isTouch3x == -3)
    {
        moving = 0;
        minimap_shadow = minimap;
        tag_edge_moving = 0;
    }
    if (expand && ges->isTouch3x == -3)
    {
        expand = 0;
        minimap_shadow = minimap;
    }

    if (!expand && !moving && ges->tagPressed >= tag_border_start && ges->tagPressed <= tag_border_end)
    {
        // expand mode
        expand = 1;
        tag_edge_selected = ges->tagPressed;
        tag_edge_moving = ges->tagPressed;
    }

    if (!expand && ges->tagPressed == tag_camera)
    {
        // moving mode
        moving = 1;
    }

    if (expand)
    {
        if (tag_edge_moving == tag_border_tl)
        {
            minimap.x = max(CAM_MINX, minimap_shadow.x + ges->distanceX);
            minimap.y = max(CAM_MINY, minimap_shadow.y + ges->distanceY);

            minimap.w = min(1920, max(CAM_MINW, minimap_shadow.w - ges->distanceX));
            minimap.h = min(1080, max(CAM_MINH, minimap_shadow.h - ges->distanceY));
        }
        else if (tag_edge_moving == tag_border_tr)
        {
            minimap.y = max(CAM_MINY, minimap_shadow.y + ges->distanceY);

            minimap.w = min(1920, max(CAM_MINW, minimap_shadow.w + ges->distanceX));
            minimap.h = min(1080, max(CAM_MINH, minimap_shadow.h - ges->distanceY));
        }
        else if (tag_edge_moving == tag_border_bl)
        {
            minimap.x = max(CAM_MINX, minimap_shadow.x + ges->distanceX);

            minimap.w = min(1920, max(CAM_MINW, minimap_shadow.w - ges->distanceX));
            minimap.h = min(1080, max(CAM_MINH, minimap_shadow.h + ges->distanceY));
        }
        else if (tag_edge_moving == tag_border_br)
        {
            minimap.w = min(1920, max(CAM_MINW, minimap_shadow.w + ges->distanceX));
            minimap.h = min(1080, max(CAM_MINH, minimap_shadow.h + ges->distanceY));
        }

        uint32_t minimap_scale_x = minimap.w * 100 / lvdsrx_w;
        uint32_t minimap_scale_y = minimap.h * 100 / lvdsrx_h;
        minimap_scale = max(minimap_scale_x, minimap_scale_y);

        // position adjustment
        if (!dynamic_point)
        {
            minimap.w = lvdsrx_w * minimap_scale / 100;
            minimap.h = lvdsrx_h * minimap_scale / 100;
        }
        minimap.x = min(lvdsrx_w - 100, minimap.x);
        minimap.y = min(lvdsrx_h - 100, minimap.y);
    }
    else if (moving)
    {
        minimap.x = min((int)((int)lvdsrx_w - 100), max(CAM_MINX, minimap_shadow.x + ges->distanceX));
        minimap.y = min((int)((int)lvdsrx_h - 100), max(CAM_MINY, minimap_shadow.y + ges->distanceY));
    }
    return PAGE_CONTINUE;
}

static int event_on_screenshot_ramg()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    if (ges->tagReleased != tag_btn_capture || buffer_viewing != -1)
    {
        return PAGE_CONTINUE;
    }

    buffer_onscreen_t *buffer_active = &buffer_onscreen[buffer_nth_selected];

    // render only area of screen
    box scissor;
    scissor.x = max(0, minimap.x);
    scissor.y = max(0, minimap.y);
    scissor.w = max(0, minimap.w);
    scissor.h = max(0, minimap.h);
    if (minimap.x < 0)
    {
        scissor.w += minimap.x;
    }
    if (minimap.y < 0)
    {
        scissor.h += minimap.y;
    }
    scissor.w = (scissor.w + 127) & ~127;
    scissor.w = max(0, scissor.w);

    scissor.h = ((scissor.w * scissor.h + 127) & ~127) / scissor.w;
    scissor.h = max(0, scissor.h);

    cmd_rendertarget(PHOST, buffer_active->ptr, RGB8, scissor.w, scissor.h);

    buffer_active->x = minimap.x;
    buffer_active->y = minimap.y;
    buffer_active->w = scissor.w;
    buffer_active->h = scissor.h;
    buffer_active->zoom = 100;
    buffer_active->wzoom = scissor.w;
    buffer_active->hzoom = scissor.h;

    Tag(PHOST, tag_camera);
    cmd_setbitmap(PHOST, lvds_input_addr, RGB8, lvdsrx_w, lvdsrx_h);
    Begin(PHOST, BITMAPS);
    Vertex2f(PHOST, -scissor.x, -scissor.y);
    EVE_CoDl_display(PHOST);
    EVE_CoCmd_swap(PHOST);
    EVE_CoCmd_graphicsfinish(PHOST);
    EVE_Cmd_waitFlush(PHOST);
    buffer_active->initialized = 1;

#define PIXEL_DUAL_2 3
    utils_scanout(&Swapchain_Render_Engine, ramg_render_buffer_addr, PIXEL_DUAL_2, RGB8);

    utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
    vertex_format(PHOST, 1);
    return PAGE_CONTINUE;
}

static int event_on_select_buffer()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    int *buffer_tags[] = {
        &tag_btn_buffer1,
        &tag_btn_buffer2,
        &tag_btn_buffer3};

    for (int i = 0; i < 3; i++)
    {
        if (ges->tagPressed1 == *buffer_tags[i])
        {
            buffer_nth_selected = i;
            if (buffer_nth_selected == buffer_viewing)
            {
                buffer_viewing = -1;
            }
            else if (buffer_onscreen[buffer_nth_selected].initialized)
            {
                buffer_viewing = i;
            }
            else
            {
                buffer_viewing = -1;
            }
        }
    }
    return PAGE_CONTINUE;
}

static int event()
{
    event_on_screenshot_ramg();
    event_on_camera_scaling();
    event_on_select_buffer();

    Gesture_Touch_t *ges = utils_gestureGet();
    if (ges->tagPressed >= tag_border_start && ges->tagPressed < tag_border_end)
    {
        border_color = color_selected;
    }
    else
    {
        border_color = color_unselected;
    }
    return PAGE_CONTINUE;
}

static int drawing_options()
{
    int w = 250;
    int h = 100;
    int x = 700;
    int y = 1080 + (120 / 2 - h / 2);
    int font = 29;
    int gap = 50;
    uint16_t options = 0;

#define ENABLE_SDCARD_CAPTURE 0
#if ENABLE_SDCARD_CAPTURE
    EVE_CoCmd_fgColor(PHOST, 0xFF8C1A);
    tag(PHOST, tag_btn_save_2_sdcard);
    EVE_CoCmd_button(PHOST, x, y, w, h, font, options, "Capture to SDcard");
#endif

    color_rgb(PHOST, 255, 255, 255);

    if (buffer_viewing == -1)
    {
        EVE_CoCmd_fgColor(PHOST, 0xFF8C1A);
        tag(PHOST, tag_btn_capture);
        EVE_CoCmd_button(PHOST, x, y, w, h, font, options, "Take a screenshot");
    }
    else
    {
        EVE_CoCmd_text(PHOST, x + w / 2, y + h / 2, 29, OPT_CENTER, "Image viewer mode");
    }

    x += w + gap;
    w = 150;

    for (int i = 0; i < 3; i++)
    {
        if (buffer_nth_selected == i)
        {
            EVE_CoCmd_fgColor(PHOST, 0xfff5cc);
            int border = 4;
            utils_drawRect(x - border / 2, y - border / 2, w + border, h + border);
        }
        else
        {
            EVE_CoCmd_fgColor(PHOST, 0xe6b800);
        }

        tag(PHOST, tag_btn_buffer1 + i);

        if (buffer_onscreen[i].initialized == 0)
        {
            char msg[10] = {0};
            msg[0] = '0' + i;
            msg[1] = 0;
            EVE_CoCmd_button(PHOST, x, y, w, h, font, options, msg);
        }
        else
        {
            buffer_onscreen_t *buffer_active = &buffer_onscreen[i];
            int ratiow = buffer_active->wdest * 100 / buffer_active->w;
            int ratioh = buffer_active->hdest * 100 / buffer_active->h;
            int ratio = min(ratiow, ratioh);
            int destw = buffer_active->w * ratio / 100;
            int desth = buffer_active->h * ratio / 100;
            int destx = buffer_active->xdest + (buffer_active->wdest - destw) / 2;
            int desty = buffer_active->ydest + (buffer_active->hdest - desth) / 2;

            cmd_setbitmap(PHOST, buffer_active->ptr, RGB8, buffer_active->w, buffer_active->h);
            Begin(PHOST, BITMAPS);
            if (ratio != 100)
            {
                utils_scale(ratio);
            }
            Vertex2f(PHOST, destx, desty);
            if (ratio != 100)
            {
                utils_scale(100);
            }

            int textx = destx + destw / 2;
            int texty = desty + desth / 2;
            char msg[20];
            snprintf(msg, sizeof(msg), "%d: %dx%d", i, buffer_active->w, buffer_active->h);
            cmd_text(PHOST, textx, texty, 28, OPT_CENTER, msg);
        }
        x += w + 20;
    }

    EVE_CoCmd_fgColor(PHOST, 0x003870); // default color
}

static int drawing_buffer_viewing()
{
    if (buffer_viewing == -1 || buffer_onscreen[buffer_viewing].initialized == 0)
    {
        return 0;
    }

    buffer_onscreen_t *buffer_active = &buffer_onscreen[buffer_viewing];

    Tag(PHOST, 0);
    cmd_setbitmap(PHOST, buffer_active->ptr, RGB8, buffer_active->w, buffer_active->h);
    Begin(PHOST, BITMAPS);
    int x = lvdsrx_w / 2 - buffer_active->w / 2;
    int y = lvdsrx_h / 2 - buffer_active->h / 2;
    Vertex2f(PHOST, x, y);
    return 1;
}

static int drawing_buffer_active()
{
    buffer_onscreen_t *buffer_active = &buffer_onscreen[buffer_nth_selected];
    if (buffer_active->initialized == 0)
    {
        return 0;
    }

    Tag(PHOST, 0);
    cmd_setbitmap(PHOST, buffer_active->ptr, RGB8, buffer_active->w, buffer_active->h);
    Begin(PHOST, BITMAPS);
    if (buffer_active->zoom != 100)
    {
        utils_scale(buffer_active->zoom);
    }
    Vertex2f(PHOST, buffer_active->x, buffer_active->y);
    if (buffer_active->zoom != 100)
    {
        utils_scale(100);
    }
    int textx = buffer_active->x + (buffer_active->wzoom) / 2;
    int texty = buffer_active->y + (buffer_active->hzoom) / 2;
    char msg[20];
    snprintf(msg, sizeof(msg), "%d: %dx%d", buffer_nth_selected, buffer_active->w, buffer_active->h);
    cmd_text(PHOST, textx, texty, 28, OPT_CENTER, msg);

    // find the final zooming ratio
    int ratiow = buffer_active->wdest * 100 / buffer_active->w;
    int ratioh = buffer_active->hdest * 100 / buffer_active->h;
    int ratio = min(ratiow, ratioh);
    int destw = buffer_active->w * ratio / 100;
    int desth = buffer_active->h * ratio / 100;

    const int zoom_speed = 8;
    // scale buffer to desired size
    if (buffer_active->zoom - ratio >= zoom_speed)
    {
        buffer_active->zoom -= zoom_speed;
    }
    else if (ratio - buffer_active->zoom >= zoom_speed)
    {
        buffer_active->zoom += zoom_speed;
    }
    else
    {
        buffer_active->zoom = ratio;
    }

    buffer_active->wzoom = buffer_active->zoom * buffer_active->w / 100;
    buffer_active->hzoom = buffer_active->zoom * buffer_active->h / 100;

    // moving buffer to desired position
    const int speed = 35;
    int destx = buffer_active->xdest + (buffer_active->wdest - destw) / 2;
    int desty = buffer_active->ydest + (buffer_active->hdest - desth) / 2;

    if (buffer_active->x - destx >= speed)
    {
        buffer_active->x -= speed;
    }
    else if (destx - buffer_active->x >= speed)
    {
        buffer_active->x += speed;
    }
    else
    {
        buffer_active->x = destx;
    }

    if (buffer_active->y - desty >= speed)
    {
        buffer_active->y -= speed;
    }
    else if (desty - buffer_active->y >= speed)
    {
        buffer_active->y += speed;
    }
    else
    {
        buffer_active->y = desty;
    }
}
static int drawing_minimap()
{
    int border = 6;
    Tag(PHOST, tag_camera);
    cmd_setbitmap(PHOST, lvds_input_addr, RGB8, lvdsrx_w, lvdsrx_h);
    Begin(PHOST, BITMAPS);
    Vertex2f(PHOST, 0, 0);
}

static int drawing_1edge(int x, int y, int w, int h, int tag)
{
    if (tag_edge_selected == tag)
    {
        color_rgb(PHOST, 255, 0, 0);
    }
    else
    {
        color_a(PHOST, 100);
    }
    tag(PHOST, tag);
    begin(PHOST, RECTS);
    vertex2f(PHOST, x, y);
    vertex2f(PHOST, x + w, y + h);
    if (tag_edge_selected == tag)
    {
        color_rgb(PHOST, 255, 255, 255);
    }
    else
    {
        color_a(PHOST, 255);
    }
}
static int drawing()
{
    int border = 10, bx = minimap.x, by = minimap.y, bw = minimap.w, bh = minimap.h;
#define ENABLE_SCISSOR 0
#if ENABLE_SCISSOR
    {
        box scissor;
        scissor.x = max(0, bx);
        scissor.y = max(0, by);
        scissor.w = max(0, bw);
        scissor.h = max(0, bh);
        if (bx < 0)
        {
            scissor.w += bx;
        }
        if (by < 0)
        {
            scissor.h += by;
        }
        scissor.w = max(0, scissor.w);
        scissor.h = max(0, scissor.h);

        EVE_Cmd_wr32(PHOST, SCISSOR_XY(scissor.x, scissor.y));
        EVE_Cmd_wr32(PHOST, SCISSOR_SIZE(scissor.w, scissor.h));
    }
#endif

    if (buffer_viewing == -1)
    {
        drawing_minimap();

#define ENABLE_BORDER_CAMERA 1
#if ENABLE_BORDER_CAMERA
        // Camera border
        // start draw a small rectangle with alpha value 255
        save_context(PHOST);
        color_mask(PHOST, 0, 0, 0, 1);
        blend_func(PHOST, ONE, ZERO);
        color_a(PHOST, 0);
        begin(PHOST, RECTS);
        vertex2f(PHOST, bx + border, by + border);
        vertex2f(PHOST, bx + bw - border * 2, by + bh - border * 2);
        // finally draw the bigger rectange with the desired rgb value
        color_rgb(PHOST, border_color[0], border_color[1], border_color[2]);
        color_mask(PHOST, 1, 1, 1, 1);
        blend_func(PHOST, DST_ALPHA, ONE_MINUS_DST_ALPHA);
        tag(PHOST, tag_camera);
        begin(PHOST, RECTS);
        vertex2f(PHOST, bx, by);
        vertex2f(PHOST, bx + bw, by + bh);
        restore_context(PHOST);
#endif
        color_rgb(PHOST, border_color[0], border_color[1], border_color[2]);
        int32_t tag_size = 100;
        int32_t tag_line_size_w = bw - tag_size * 2;
        int32_t tag_line_size_h = bh - tag_size * 2;
        int32_t tag_line_size_thin = 20;

        utils_scale(100);
        // touch area top left
        drawing_1edge(bx, by, tag_size, tag_size, tag_border_tl);
        // touch area top right
        drawing_1edge(bx + bw - tag_size, by, tag_size, tag_size, tag_border_tr);
        // touch area bottom left
        drawing_1edge(bx, by + bh - tag_size, tag_size, tag_size, tag_border_bl);
        // touch area bottom right
        drawing_1edge(bx + bw - tag_size, by + bh - tag_size, tag_size, tag_size, tag_border_br);

#define ENABLE_BORDER_TOUCH 0
#if ENABLE_BORDER_TOUCH
        // touch area left
        drawing_1edge(bx, by + tag_size, tag_line_size_thin, tag_line_size_h, tag_border_l);
        // touch area right
        drawing_1edge(bx + bw - tag_line_size_thin, by + tag_size, tag_line_size_thin, tag_line_size_h, tag_border_r);
        // touch area top
        drawing_1edge(bx + tag_size, by, tag_line_size_w, tag_line_size_thin, tag_border_t);
        // touch area bottom
        drawing_1edge(bx + tag_size, by + bh - tag_size, tag_line_size_w, tag_line_size_thin, tag_border_b);
        color_rgb(PHOST, 255, 255, 255);
#endif

#if ENABLE_SCISSOR
        EVE_Cmd_wr32(PHOST, SCISSOR_XY(0, 0));
        EVE_Cmd_wr32(PHOST, SCISSOR_SIZE(2048, 2048));
#endif
        color_rgb(PHOST, 255, 255, 255);
        utils_drawImageXY(PHOST, IMG_TASKBAR, 0, 1080);
        drawing_buffer_active();
    }
    else
    {
        utils_drawImageXY(PHOST, IMG_TASKBAR, 0, 1080);
        drawing_buffer_viewing();
    }

    drawing_options();

    return 0;
}

static void page_screenshot_init_one()
{
    static bool is_page_screenshot_init = 0;
    if (is_page_screenshot_init == 1)
        return;
    is_page_screenshot_init = 1;

    tag_border_tl = utils_tagAlloc(tag_border_tl);
    tag_border_tr = utils_tagAlloc(tag_border_tr);
    tag_border_bl = utils_tagAlloc(tag_border_bl);
    tag_border_br = utils_tagAlloc(tag_border_br);
    tag_border_l = utils_tagAlloc(tag_border_l);
    tag_border_r = utils_tagAlloc(tag_border_r);
    tag_border_t = utils_tagAlloc(tag_border_t);
    tag_border_b = utils_tagAlloc(tag_border_b);

    tag_border_start = tag_border_tl;
    tag_border_end = tag_border_b;

    tag_camera = utils_tagAlloc(TAG_INC);
    tag_btn_save_2_sdcard = utils_tagAlloc(TAG_INC);
    tag_btn_capture = utils_tagAlloc(TAG_INC);
    tag_btn_buffer1 = utils_tagAlloc(TAG_INC);
    tag_btn_buffer2 = utils_tagAlloc(TAG_INC);
    tag_btn_buffer3 = utils_tagAlloc(TAG_INC);

    minimap.x = 0;
    minimap.y = 0;
    minimap.w = lvdsrx_w;
    minimap.h = lvdsrx_h;

    minimap_shadow = minimap;
    minimap_scale = 100;

    ramg_render_screenshot_addr = utils_ddrAllocAlignment(2048 * 2048 * 2, 128);

    int w = 250;
    int h = 100;
    int x = 700 + w + 50;
    int y = 1080 + (120 / 2 - h / 2);
    w = 150;
    for (int i = 0; i < 3; i++)
    {
        buffer_onscreen[i].ptr = utils_ddrAllocAlignment(2048 * 2048 * 2, 128);
        buffer_onscreen[i].initialized = 0;
        buffer_onscreen[i].zoom = 100;
        buffer_onscreen[i].xdest = x;
        buffer_onscreen[i].ydest = y;
        buffer_onscreen[i].wdest = w;
        buffer_onscreen[i].hdest = h;
        x += w + 20;
    }
}

static void page_screenshot_deinit_one()
{
    static bool is_page_screenshot_deinit = 0;
    if (is_page_screenshot_deinit == 1)
        return;
    is_page_screenshot_deinit = 1;
}

void page_screenshot_load()
{
}

int page_screenshot_draw()
{
    int ret = event();
    drawing();
    return ret;
}

void page_screenshot_init()
{
    page_screenshot_init_one();

    minimap.x = 0;
    minimap.y = 0;
    minimap.w = lvdsrx_w;
    minimap.h = lvdsrx_h;

    minimap_shadow = minimap;
    minimap_scale = 100;

    buffer_viewing = -1;

    int w = 250;
    int h = 100;
    int x = 700 + w + 50;
    int y = 1080 + (120 / 2 - h / 2);
    w = 150;
    for (int i = 0; i < 3; i++)
    {
        buffer_onscreen[i].initialized = 0;
        buffer_onscreen[i].zoom = 100;
        buffer_onscreen[i].xdest = x;
        buffer_onscreen[i].ydest = y;
        buffer_onscreen[i].wdest = w;
        buffer_onscreen[i].hdest = h;
        x += w + 20;
    }
}

void page_screenshot_deinit()
{
    page_screenshot_deinit_one();
}
