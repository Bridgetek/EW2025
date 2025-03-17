/**
 * @file Page_Zoom.c
 * @brief Zoom page
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

static box lvdsrx_map_copy = {0, 0, 1920, 1080};
static uint32_t tag_camera = 0;
extern uint32_t lvds_input_addr; // Data address pull from EVE
extern uint32_t taskbar_icon_x;
extern uint32_t taskbar_icon_y;

#define MAX_ZOOM_LEVEL 800
#define MIN_ZOOM_LEVEL 10

int CAM_MINX = 0;
int CAM_MINY = 0;
int CAM_MINW = 0;
int CAM_MINH = 0;

static uint32_t tag_btn_default = 0;
static uint32_t tag_font_zoomup = 0;
static uint32_t tag_font_zoomin = 0;
static uint32_t tag_font_zoomout = 0;
static uint32_t tag_font_zoom_slider = 0;

static bool moving = 0, expand = 0;

static bool dynamic_point = 1;

extern box lvdsrx_map;
extern uint32_t lvdsrx_scale;

static int slider_val = 0;

static box sliderbox = {650, 1100, 500, 30};

#define ZOOM_LIST_CLOSE_Y 1100
static int zoom_list_open_y = 0;
static int zoom_list_x = 650;
static int zoom_list_y = 0;
static int zoom_list_w = 100;
static int zoom_list_h = 50;
static int zoom_list_y_dest = ZOOM_LIST_CLOSE_Y;

char *zoom_lv_list_s[] = {"10%", "25%", "50%", "75%", "100%", "200%", "300%", "400%", "800%"};
int zoom_lv_list_d[] = {10, 25, 50, 75, 100, 200, 300, 400, 800};
int zoom_lv_list_tag[] = {10, 25, 50, 75, 100, 200, 300, 400, 800};
int zoom_lv_num = sizeof(zoom_lv_list_d) / sizeof(zoom_lv_list_d[0]);

static int event_on_camera_scaling()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    tag_edge_selected = 0;

    if (ges->tagPressed1 == tag_font_zoomup)
    {
        if (zoom_list_y_dest == ZOOM_LIST_CLOSE_Y)
        {
            zoom_list_y_dest = zoom_list_open_y;
            zoom_list_y = ZOOM_LIST_CLOSE_Y;
        }
        else
        {
            zoom_list_y_dest = ZOOM_LIST_CLOSE_Y;
            zoom_list_y = zoom_list_open_y;
        }
    }

    for (int i = 0; i < zoom_lv_num; i++)
    {
        if (ges->tagPressed1 == zoom_lv_list_tag[i])
        {
            lvdsrx_scale = zoom_lv_list_d[i];

            lvdsrx_map.x = 0;
            lvdsrx_map.y = 0;
            lvdsrx_map.w = lvdsrx_w * lvdsrx_scale / 100;
            lvdsrx_map.h = lvdsrx_h * lvdsrx_scale / 100;

            lvdsrx_map_copy = lvdsrx_map;

            zoom_list_y = ZOOM_LIST_CLOSE_Y;
            zoom_list_y_dest = ZOOM_LIST_CLOSE_Y;
            moving = 0;
            expand = 0;
            return;
        }
    }

    if (ges->tagPressed1 == tag_btn_default && !moving && !expand)
    {
        lvdsrx_map.x = 0;
        lvdsrx_map.y = 0;
        lvdsrx_map.w = lvdsrx_w;
        lvdsrx_map.h = lvdsrx_h;

        lvdsrx_map_copy = lvdsrx_map;
        lvdsrx_scale = 100;

        slider_val = 65535;
    }

    if (moving && ges->isTouch3x == -3)
    {
        moving = 0;
        lvdsrx_map_copy = lvdsrx_map;
        tag_edge_moving = 0;
    }
    if (expand && ges->isTouch3x == -3)
    {
        expand = 0;
        lvdsrx_map_copy = lvdsrx_map;
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
            lvdsrx_map.x = max(CAM_MINX, lvdsrx_map_copy.x + ges->distanceX);
            lvdsrx_map.y = max(CAM_MINY, lvdsrx_map_copy.y + ges->distanceY);

            lvdsrx_map.w = max(CAM_MINW, lvdsrx_map_copy.w - ges->distanceX);
            lvdsrx_map.h = max(CAM_MINH, lvdsrx_map_copy.h - ges->distanceY);
        }
        else if (tag_edge_moving == tag_border_tr)
        {
            lvdsrx_map.y = max(CAM_MINY, lvdsrx_map_copy.y + ges->distanceY);

            lvdsrx_map.w = max(CAM_MINW, lvdsrx_map_copy.w + ges->distanceX);
            lvdsrx_map.h = max(CAM_MINH, lvdsrx_map_copy.h - ges->distanceY);
        }
        else if (tag_edge_moving == tag_border_bl)
        {
            lvdsrx_map.x = max(CAM_MINX, lvdsrx_map_copy.x + ges->distanceX);

            lvdsrx_map.w = max(CAM_MINW, lvdsrx_map_copy.w - ges->distanceX);
            lvdsrx_map.h = max(CAM_MINH, lvdsrx_map_copy.h + ges->distanceY);
        }
        else if (tag_edge_moving == tag_border_br)
        {
            lvdsrx_map.w = max(CAM_MINW, lvdsrx_map_copy.w + ges->distanceX);
            lvdsrx_map.h = max(CAM_MINH, lvdsrx_map_copy.h + ges->distanceY);
        }

        uint32_t lvdsrx_scale_x = lvdsrx_map.w * 100 / lvdsrx_w;
        uint32_t lvdsrx_scale_y = lvdsrx_map.h * 100 / lvdsrx_h;
        lvdsrx_scale = max(lvdsrx_scale_x, lvdsrx_scale_y);
        slider_val = lvdsrx_scale * 65535 / 100;

        // position adjustment
        lvdsrx_map.w = lvdsrx_w * lvdsrx_scale / 100;
        lvdsrx_map.h = lvdsrx_h * lvdsrx_scale / 100;
        lvdsrx_map.x = min(lvdsrx_w - 100, lvdsrx_map.x);
        lvdsrx_map.y = min(lvdsrx_h - 100, lvdsrx_map.y);
    }
    else if (moving)
    {
        lvdsrx_map.x = min(lvdsrx_w - 100, max(CAM_MINX, lvdsrx_map_copy.x + ges->distanceX));
        lvdsrx_map.y = min(lvdsrx_h - 100, max(CAM_MINY, lvdsrx_map_copy.y + ges->distanceY));
    }
    return PAGE_CONTINUE;
}

static int event()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    event_on_camera_scaling();
    if (ges->tagPressed >= tag_border_start && ges->tagPressed < tag_border_end)
    {
        border_color = color_selected;
    }
    else
    {
        border_color = color_unselected;
    }

    if (tag_font_zoom_slider == ges->tagPressed)
    {
        int distance = ges->touchX - sliderbox.x;
        slider_val = max(0, min(65535, distance * 65535 / sliderbox.w));

        lvdsrx_scale = max(MIN_ZOOM_LEVEL, min(MAX_ZOOM_LEVEL, slider_val * MAX_ZOOM_LEVEL / 65535));
        lvdsrx_map.w = lvdsrx_w * lvdsrx_scale / 100;
        lvdsrx_map.h = lvdsrx_h * lvdsrx_scale / 100;
        lvdsrx_map_copy = lvdsrx_map;

        expand = 0;
    }
    else if (ges->tagPressed1 == tag_font_zoomin)
    {
        for (int i = 0; i < zoom_lv_num; i++)
        {
            if (zoom_lv_list_d[i] > lvdsrx_scale)
            {
                lvdsrx_scale = zoom_lv_list_d[i];
                break;
            }
        }
    }
    else if (ges->tagPressed1 == tag_font_zoomout)
    {
        for (int i = zoom_lv_num - 1; i >= 0; i--)
        {
            if (zoom_lv_list_d[i] < lvdsrx_scale)
            {
                lvdsrx_scale = zoom_lv_list_d[i];
                break;
            }
        }
    }
    return PAGE_CONTINUE;
}

static int drawing_options_zoom_list()
{
    const int speed = 3;
    const int velocity = 3;
    const int enable_ui_test = 0;

    if (zoom_list_y - zoom_list_y_dest > speed)
    {
        zoom_list_y -= speed + (zoom_list_y - zoom_list_y_dest) / velocity;
    }
    else if (zoom_list_y_dest - zoom_list_y > speed)
    {
        zoom_list_y += speed + (zoom_list_y_dest - zoom_list_y) / velocity;
    }
    else if (zoom_list_y_dest == zoom_list_open_y)
    {
        zoom_list_y = zoom_list_y_dest;

        if (enable_ui_test)
            zoom_list_y_dest = ZOOM_LIST_CLOSE_Y;
    }
    else
    {
        if (enable_ui_test)
            zoom_list_y_dest = zoom_list_open_y;
        else
            return;
    }

    // draw the list
    int row_w = zoom_list_w;
    int row_h = 50;
    box zoomlevel = {zoom_list_x, zoom_list_y, row_w, row_h * zoom_lv_num};
    int scissor_h = max(0, min(zoomlevel.h, ZOOM_LIST_CLOSE_Y - zoom_list_y));
    scissor_xy(PHOST, zoomlevel.x, zoomlevel.y);
    scissor_size(PHOST, zoomlevel.w, scissor_h);
    utils_drawRect(zoom_list_x, zoom_list_y, row_w, row_h * zoom_lv_num);

    // draw the text
    color_rgb(PHOST, 0, 0, 0);
    for (int i = 0; i < zoom_lv_num; i++)
    {
        int x = zoom_list_x;
        int y = zoom_list_y + i * row_h;

        if (zoom_lv_list_d[i] == lvdsrx_scale)
        {
            color_rgb(PHOST, 0xb3, 0xb3, 0xb3);
        }
        else
        {
            color_rgb(PHOST, 0, 0, 0);
        }
        tag(PHOST, zoom_lv_list_tag[i]);
        utils_drawRect(x, y, row_w, row_h);

        color_rgb(PHOST, 255, 255, 255);
        tag(PHOST, zoom_lv_list_tag[i]);
        cmd_text(PHOST, x + zoom_list_w / 2, y + zoom_list_h / 2, 28, OPT_CENTER, zoom_lv_list_s[i]);
    }
    color_rgb(PHOST, 255, 255, 255);

    scissor_xy(PHOST, 0, 0);
    scissor_size(PHOST, 2048, 2048);
}
static int drawing_options()
{
    int offset_x = taskbar_icon_x;
    int offset_y = taskbar_icon_y;
    int w = 150;
    int h = 70;
    int x = 650;

    int y = 1080 + (120 / 2 - h / 2);
    int font = 29;
    uint16_t options = 0;

    EVE_CoCmd_fgColor(PHOST, 0xFF8C1A);

    char msg[100];
    snprintf(msg, 100, "%d%%", lvdsrx_scale);
    slider_val = max(MIN_ZOOM_LEVEL, min(65535, lvdsrx_scale * 65535 / MAX_ZOOM_LEVEL));

    // zoom level text
    box zoomlevel = {x, y, zoom_list_w, 70};
    utils_drawBoxTextLeft(PHOST, x, y, zoomlevel.w, zoomlevel.h, 0, (char[]){0, 0, 255}, tag_font_zoomup, msg, 29, 0, (char[]){255, 255, 255});

    x += zoomlevel.w - IMG_ICON_UPARROW->w - 2;
    utils_drawImageXyTag(PHOST, IMG_ICON_UPARROW, x, y + 5, tag_font_zoomup);

    // zoom in button
    x += IMG_ICON_UPARROW->w + 5 + 10;
    Image *icon = IMG_ICON_ZOOMOUT;
    utils_drawImageXyTag(PHOST, icon, x, y, tag_font_zoomout);

    // zoom slider
    if (lvdsrx_scale == MIN_ZOOM_LEVEL)
    {
        color_rgb(PHOST, 0xb3, 0xb3, 0xb3);
    }
    x += icon->w + 10;
    sliderbox.x = x;
    tag(PHOST, tag_font_zoom_slider);
    EVE_CoCmd_slider(PHOST, sliderbox.x, sliderbox.y + 20, sliderbox.w, sliderbox.h, 0, slider_val, 65535);
    if (lvdsrx_scale == MIN_ZOOM_LEVEL)
    {
        color_rgb(PHOST, 255, 255, 255);
    }
    // slider touch tracking area
    color_a(PHOST, 0);
    tag(PHOST, tag_font_zoom_slider);
    utils_drawRect(sliderbox.x, sliderbox.y - 20, sliderbox.w, sliderbox.h + 100);
    color_a(PHOST, 255);

    // zoom out
    x += sliderbox.w + 20;
    icon = IMG_ICON_ZOOMIN;
    utils_drawImageXyTag(PHOST, icon, x, y, tag_font_zoomin);

    x += icon->w + 10;
    tag(PHOST, tag_btn_default);
    EVE_CoCmd_button(PHOST, x, y, w, h, font, options, "Default");

    drawing_options_zoom_list();
}

static int drawing_lvdsrx_map()
{
    if (lvdsrx_scale != 100)
    {
        cmd_loadidentity(PHOST);
        EVE_CoCmd_scale(PHOST, 65536 * lvdsrx_scale / 100, 65536 * lvdsrx_scale / 100);
    }

    Tag(PHOST, tag_camera);
    cmd_setbitmap(PHOST, lvds_input_addr, RGB8, lvdsrx_w, lvdsrx_h);
    Begin(PHOST, BITMAPS);
    if (lvdsrx_scale > 100)
    {
        int32_t frameW = lvdsrx_w * lvdsrx_scale / 100;
        int32_t frameH = lvdsrx_h * lvdsrx_scale / 100;
        bitmap_size(PHOST, NEAREST, BORDER, BORDER, 0, 0);
        bitmap_size_h(PHOST, 0, 0);
        cmd_translate(PHOST, lvdsrx_map.x * 65536, lvdsrx_map.y * 65536);
        cmd_setmatrix(PHOST);
        Vertex2f(PHOST, 0, 0);
    }
    else if (lvdsrx_scale != 100)
    {
        cmd_setmatrix(PHOST);
        Vertex2f(PHOST, lvdsrx_map.x, lvdsrx_map.y);
    }
    else
    {
        Vertex2f(PHOST, lvdsrx_map.x, lvdsrx_map.y);
    }

    if (lvdsrx_scale != 100)
    {
        utils_scale(100);
    }
}

static int drawing_1edge(int x, int y, int w, int h, int tag)
{
    if (tag_edge_moving == tag)
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
    if (tag_edge_moving == tag)
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
    int border = 10, bx = lvdsrx_map.x, by = lvdsrx_map.y, bw = lvdsrx_map.w, bh = lvdsrx_map.h;
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

    drawing_lvdsrx_map();

#define ENABLE_BORDER_CAMERA 0
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

#define ENABLE_ANGLE_DRAWING 0
#if ENABLE_ANGLE_DRAWING
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
#endif

#define ENABLE_EDGE_DRAWING 0
#if ENABLE_EDGE_DRAWING
    // touch area left
    drawing_1edge(bx, by + tag_size, tag_line_size_thin, tag_line_size_h, tag_border_l);
    // touch area right
    drawing_1edge(bx + bw - tag_line_size_thin, by + tag_size, tag_line_size_thin, tag_line_size_h, tag_border_r);
    // touch area top
    drawing_1edge(bx + tag_size, by, tag_line_size_w, tag_line_size_thin, tag_border_t);
    // touch area bottom
    drawing_1edge(bx + tag_size, by + bh - tag_size, tag_line_size_w, tag_line_size_thin, tag_border_b);
#endif

#if ENABLE_SCISSOR
    EVE_Cmd_wr32(PHOST, SCISSOR_XY(0, 0));
    EVE_Cmd_wr32(PHOST, SCISSOR_SIZE(2048, 2048));
#endif

    utils_drawImageXY(PHOST, IMG_TASKBAR, 0, 1080);
    drawing_options();
    return 0;
}

static void page_zoom_init_one()
{
    static bool is_page_zoom_init = 0;
    if (is_page_zoom_init == 1)
        return;
    is_page_zoom_init = 1;

    CAM_MINX = -1 * (lvdsrx_w * MAX_ZOOM_LEVEL / 100);
    CAM_MINY = -1 * (lvdsrx_h * MAX_ZOOM_LEVEL / 100);
    CAM_MINW = lvdsrx_w * MIN_ZOOM_LEVEL / 100;
    CAM_MINH = lvdsrx_h * MIN_ZOOM_LEVEL / 100;

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
    tag_btn_default = utils_tagAlloc(TAG_INC);
    tag_font_zoomup = utils_tagAlloc(TAG_INC);
    tag_font_zoomin = utils_tagAlloc(TAG_INC);
    tag_font_zoomout = utils_tagAlloc(TAG_INC);
    tag_font_zoom_slider = utils_tagAlloc(TAG_INC);

    lvdsrx_map.x = 0;
    lvdsrx_map.y = 0;
    lvdsrx_map.w = lvdsrx_w;
    lvdsrx_map.h = lvdsrx_h;

    lvdsrx_map_copy = lvdsrx_map;
    lvdsrx_scale = 100;

    zoom_list_open_y = ZOOM_LIST_CLOSE_Y - zoom_lv_num * zoom_list_h;
    zoom_list_y = zoom_list_open_y;

    for (int i = 0; i < zoom_lv_num; i++)
    {
        zoom_lv_list_tag[i] = utils_tagAlloc(TAG_INC);
    }
}

static void page_zoom_deinit_one()
{
    static bool is_page_zoom_deinit = 0;
    if (is_page_zoom_deinit == 1)
        return;
    is_page_zoom_deinit = 1;
}

void page_zoom_load()
{
}

int page_zoom_draw()
{
    int ret = event();
    drawing();
    return ret;
}

void page_zoom_init()
{
    page_zoom_init_one();
}

void page_zoom_deinit()
{
    page_zoom_deinit_one();

    lvdsrx_scale = zoom_lv_list_d[4];

    lvdsrx_map.x = 0;
    lvdsrx_map.y = 0;
    lvdsrx_map.w = lvdsrx_w * lvdsrx_scale / 100;
    lvdsrx_map.h = lvdsrx_h * lvdsrx_scale / 100;

    lvdsrx_map_copy = lvdsrx_map;

    zoom_list_y = ZOOM_LIST_CLOSE_Y;
    zoom_list_y_dest = ZOOM_LIST_CLOSE_Y;
    moving = 0;
    expand = 0;
}
