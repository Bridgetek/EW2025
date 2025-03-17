/**
 * @file Page_Taskbar.c
 * @brief Draw the taskbar
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

static Timer timer_opc = {{0}};
static Timer timer_menu_auto_hide = {{0}};
static uint32_t text_menu_opc = 255;
static int32_t text_menu_opc_step = 2;
static int32_t text_menu_opc_inc = 2;

static int hover_at = 0;
bool g_is_menu_active = 0;
static bool is_main_menu_taskbar = 0;
static int tag_close = 0;

typedef struct
{
    utils_paging *page;
    Image *icon;
} menu_item_t;

static menu_item_t menu_pages[] = {
    {&page_camera_effect, IMG_ICON_EFFECT},
    {&page_keyboard, IMG_ICON_KEYBOARD},
    {&page_playback, IMG_ICON_PLAYBACK},
    {&page_sketch, IMG_ICON_SKETCH},
    {&page_sticker, IMG_ICON_STICKER},
    {&page_zoom, IMG_ICON_ZOOM},
    {&page_screenshot, IMG_ICON_CROP},
    {&page_glow, IMG_ICON_GLOW},
};

static int num_pages = sizeof(menu_pages) / sizeof(menu_pages[0]);

const uint32_t taskbar_icon_x = 650;
const uint32_t taskbar_icon_y = 1090;
extern uint32_t ramg_render_buffer_addr;

/**
 * @brief Deselect all pages
 *
 * Deselect all pages and deinit them
 */
static void deselect_all()
{
    for (int i = 0; i < num_pages; i++)
    {
        utils_paging *p = menu_pages[i].page;
        p->isActive = false;
        p->isInitialized = false;
        p->deinit();

        if (menu_pages[i].page == &page_camera_effect)
        {
        }
    }
}

/**
 * @brief Blinking effect for menu bar
 *
 * Blinking effect for menu bar, only blink when menu is not active
 */
static int event_menu_blinking()
{
    if (g_is_menu_active)
    {
        text_menu_opc = 255;
    }
    else if (utils_timerIsExpired(&timer_opc))
    {
        if (abs(text_menu_opc_inc) < 200)
        {
            text_menu_opc_step = 20;
        }
        else
        {
            text_menu_opc_step = 3;
        }

        text_menu_opc += text_menu_opc_inc;
        if (text_menu_opc >= 255)
        {
            text_menu_opc_inc = -text_menu_opc_step;
        }
        else if (text_menu_opc <= 0)
        {
            text_menu_opc_inc = text_menu_opc_step;
        }
    }
    return PAGE_CONTINUE;
}

/**
 * @brief Check if a menu item is selected
 *
 * Iterate through all menu items. If any menu item is selected, set it to active and
 * return PAGE_CONTINUE. If no menu item is selected, return 0.
 *
 * @return 0 if no menu item is selected, PAGE_CONTINUE if a menu item is selected
 */
static int event_menu_selected(void)
{
    Gesture_Touch_t *ges = utils_gestureGet();
    for (int i = 0; i < num_pages; i++)
    {
        utils_paging *page = menu_pages[i].page;
        Image *icon = menu_pages[i].icon;
        if (ges->tagReleased == icon->tagval)
        {
            deselect_all();
            page->isActive = 1;

            return PAGE_CONTINUE;
        }
    }
    return PAGE_CONTINUE;
}

/**
 * @brief Handle gesture events and update menu states
 *
 * This function processes gesture input to manage menu interactions.
 * It handles the release of gesture tags, toggles menu activity,
 * and updates the hover state. If the close tag is released, all pages
 * are deselected. The function also checks if the main menu icon is
 * activated and toggles the menu's active state accordingly.
 *
 * @return PAGE_CONTINUE always
 */
static int event()
{
    hover_at = 0;
    Gesture_Touch_t *ges = utils_gestureGet();

    if (ges->tagReleased == 0)
    {
        return PAGE_CONTINUE;
    }
    if (ges->tagReleased == tag_close)
    {
        deselect_all();
    }

    event_menu_selected();

    if (ges->tagReleased == IMG_ICON_MENU_ON->tagval)
    {
        g_is_menu_active = !g_is_menu_active;
    }

    for (int i = 0; i < num_pages; i++)
    {
        if (ges->tagReleased == menu_pages[i].icon->tagval)
        {
            g_is_menu_active = 0;
            hover_at = ges->tagPressed;
            break;
        }
    }

    page_camera.isActive = 1;
    if (page_camera_effect.isActive && g_is_menu_active == 0)
    {
        page_camera.isActive = 0;
    }

    return PAGE_CONTINUE;
}

/**
 * @brief Draw a menu item with frame
 * @param itemimg The menu item image
 * @param x The x position of the menu item
 * @param y The y position of the menu item
 */
static void draw_menu_1page(Image *itemimg, int x, int y, int zoom, int tag)
{
    if (tag == 0)
        tag = itemimg->tagval;
    Image *frameimg = IMG_ICON_BOX;
    if (hover_at == itemimg->tagval)
    {
        frameimg = IMG_ICON_BOX_ON;
    }

    int w = frameimg->w * zoom / 100;
    int h = frameimg->h * zoom / 100;

    int centerx = x + w / 2;
    int centery = y + h / 2;
    int itemx = centerx - itemimg->w * zoom / 100 / 2;
    int itemy = centery - itemimg->h * zoom / 100 / 2;

    // use the same tag
    frameimg->tagval = itemimg->tagval;

    if (zoom != 100)
    {
        utils_scale(zoom);
    }
    color_a(PHOST, 150);
    utils_drawImageXyTag(PHOST, frameimg, x, y, tag);
    color_a(PHOST, 255);
    utils_drawImageXyTag(PHOST, itemimg, itemx, itemy, tag);
}
static void draw_menu_popup()
{
    if (g_is_menu_active == 0)
    {
        return;
    }
    Image *frameimg = IMG_ICON_BOX;
    box menu_box = {demo_geo.x, demo_geo.y, demo_geo.w, demo_geo.h};
    box menu_item_box = {0, 0, frameimg->w, frameimg->h};

    int margin = 10;

    // auto calculation
    int num_frame_per_row = menu_box.w / (menu_item_box.w + margin);
    num_frame_per_row = 4; // manually set
    int num_frame_row = (num_pages + num_frame_per_row - 1) / num_frame_per_row;
    int num_frame_col = num_frame_per_row;

    int row_width = num_frame_col * (menu_item_box.w + margin);
    int row_height = num_frame_row * (menu_item_box.h + margin);

    int offsetx = (menu_box.w - row_width) / 2;
    int offsety = (menu_box.h - row_height) / 2;

    for (int i = 0; i < num_pages; i++)
    {
        int col = i % num_frame_per_row;
        int row = i / num_frame_per_row;

        int x = offsetx + menu_box.x + col * (menu_item_box.w + margin);
        int y = offsety + menu_box.y + row * (menu_item_box.h + margin);

        draw_menu_1page(menu_pages[i].icon, x, y, 100, 0);
    }
}

static void draw_taskbar_icons()
{
    int offset_x = 500;
    int x = offset_x, y = 0, icon_w = 0, icon_h = IMG_ICON_MENU_ON->h, scale_percent = 80;
    Image *menuicon = IMG_ICON_MENU_ON;
    y = 1080 + (120 / 2 - icon_h * scale_percent / 100 / 2);

    for (int i = 0; i < num_pages; i++)
    {
        Image *icon = menu_pages[i].icon;
        utils_paging *page = menu_pages[i].page;
        if (page->isActive)
        {
            int scale_percent = 28;
            y = 1080 + (120 / 2 - icon->h * scale_percent / 100 / 2) - 16; // centering
            y = 1080 + (120 / 2 - icon->h * scale_percent / 100 / 2) - 30; // top
            icon_w = IMG_ICON_BOX->w * scale_percent / 100;
            icon_h = IMG_ICON_BOX->h * scale_percent / 100;
            // start menu icon
            EVE_Cmd_wr32(PHOST, SCISSOR_XY(x, y));
            EVE_Cmd_wr32(PHOST, SCISSOR_SIZE(icon_w, icon_h));
            draw_menu_1page(icon, x, y, scale_percent, menuicon->tagval);
            EVE_Cmd_wr32(PHOST, SCISSOR_XY(0, 0));
            EVE_Cmd_wr32(PHOST, SCISSOR_SIZE(2048, 2048));

            // application name
            utils_scale(100);
            x = x + icon_w / 2;
            tag(PHOST, 0);
            y += icon->h * scale_percent / 100;
            cmd_text(PHOST, x, y + 40, 28, OPT_CENTER, page->uiname);

            // close button
            utils_drawImageXyTag(PHOST, IMG_ICON_CLOSE,
                                 PHOST->Width - IMG_ICON_CLOSE->w - 20,
                                 1080 + 120 / 2 - menuicon->h / 2, tag_close); // draw at right bottom
            return;
        }
    }

    // start menu icon
    utils_scale(scale_percent);
    utils_drawImageXY(PHOST, menuicon, x, y);
}
static bool drawing()
{
    draw_menu_popup();
    draw_taskbar_icons();
    return 0;
}

static void page_taskbar_init_one()
{
    static bool is_page_taskbar_init = 0;
    if (is_page_taskbar_init == 1)
        return;
    is_page_taskbar_init = 1;

    IMG_ICON_MENU_ON->tagval = IMG_ICON_MENU->tagval;

    for (int i = 0; i < num_pages; i++)
    {
        menu_pages[i].icon->tagval = utils_tagAlloc(TAG_INC);
    }
    tag_close = utils_tagAlloc(TAG_INC);
}

int page_taskbar_load()
{
}

int page_taskbar_draw()
{
    int ret = event();
    drawing();
    return ret;
}

void page_taskbar_init()
{
    page_taskbar_init_one();

    uint32_t timeout = 5000, interval = 10;
    utils_timerStart(&timer_opc, timeout, interval);
}

void page_taskbar_deinit() {}
