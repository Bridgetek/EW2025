/**
 * @file EW2024_Photobooth.c
 * @brief Example code for the EW2024 photobooth
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

#define im00 "00-bridgetek-logo.png"                                //,1280,435
#define im01 "01-0taskbar.png"                                      //,1920,120
#define im02 "02-menu.png"                                          //,80,80
#define im03 "03-menu_active.png"                                   //,80,80
#define im04 "06-box-activenoto--white-large-square.png"            //,328,328
#define im05 "07-box-blue-fluent-emoji--blue-square.png"            //,328,328
#define im06 "08-camera-effect-fluent-emoji--magic-wand.png"        //,205,205
#define im07 "09-crop-ph--crop-light.png"                           //,205,205
#define im08 "10-blur-fluent-emoji-flat--vibration-mode.png"        //,205,205
#define im09 "11-glow-twemoji--bright-button.png"                   //,205,205
#define im10 "12-keyboard-fluent-emoji-high-contrast--keyboard.png" //,205,205
#define im11 "13-playback-noto--cinema.png"                         //,205,205
#define im12 "14-sketch-noto--paintbrush.png"                       //,205,205
#define im13 "15-sticker-noto--star.png"                            //,205,205
#define im14 "17-zoom-noto-v1--magnifying-glass-tilted-left.png"    //,205,205
#define im15 "18-back-noto--end-arrow.png"                          //,100, 100
#define im16 "19-Bridgetek_FA-1920.png"                             //,1920, 580
#define im17 "20-color-picker-s.png"                                //,590, 502
#define im18 "21-linethick1.png"                                    //,90, 90
#define im19 "21-linethick2.png"                                    //,90, 90
#define im20 "21-linethick3.png"                                    //,90, 90
#define im21 "21-linethick4.png"                                    //,90, 90
#define im22 "22-paint-frame-noto--artist-palette.png"              //,90, 90
#define im23 "23-clean-icon-park--clear-format.png"                 //,90, 90
#define im24 "24-cleanall-icon-park--clear-format.png"              //,90, 90
#define im25 "25-close-icon-park--close-one.png"                    //,90, 90
#define im26 "26-zoom-mode-icon-park--move.png"                     //,80, 80
#define im27 "27-move-mode-mingcute--move-fill.png"                 //,80, 80
#define im28 "28-camera-mdi--camera-outline.png"                    //,80, 80
#define im29 "29-more-ri--more-fill.png"                            //,80, 80
#define im30 "30-zoomin-ic--round-zoom-in.png"                      //,80,80
#define im31 "31-zoomout-ic--round-zoom-out.png"                    //,80,80
#define im32 "32-uparrow-mingcute--up-fill.png"                     //,40,40

Image demo_images[] = {
    // image w and h copy from command: identify -format "%f,%w,%h\r\n" *.*
    // for /R %f in (.\*) do identify -format "%f,%w,%h\r\n" %f
    {.sd_path = IMG_DIR im00, .w = 1280, .h = 435, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im01, .w = 1920, .h = 120, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im02, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im03, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im04, .w = 328, .h = 328, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im05, .w = 328, .h = 328, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im06, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im07, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im08, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im09, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im10, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im11, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im12, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im13, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im14, .w = 205, .h = 205, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im15, .w = 100, .h = 100, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im16, .w = 1920, .h = 580, .bitmap_handler = BMHL_AUTO, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im17, .w = 590, .h = 520, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im18, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im19, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im20, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im21, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im22, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im23, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im24, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im25, .w = 90, .h = 90, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im26, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im27, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im28, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im29, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im30, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im31, .w = 80, .h = 80, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
    {.sd_path = IMG_DIR im32, .w = 40, .h = 40, .bitmap_handler = BMHL_NONE, .tagval = TAG_INC, .bitmap_format = ARGB4, .fmt = IMG_FMT_PNG},
};

box demo_geo = (box){.x = 0, .y = 0, .w = 1920, .h = 1200};
box cam_geo = (box){.x = 0, .y = 0, .w = 1920, .h = 1080};
uint32_t botom_center_y  = 0;

// individual pages
PAGE(camera_effect, "Display effect")
PAGE(keyboard, "Input")
PAGE(playback, "Video")
PAGE(sketch, "Sketch overlay")
PAGE(sticker, "Sticker")
PAGE(zoom, "Zooming")
PAGE(screenshot, "Screenshot")
PAGE(glow, "Glow")

// page activated usualy
PAGE(camera, "LVDS display")
PAGE(taskbar, "Taskbar")

utils_paging *allpages[] = {
    // Display pages in order
    &page_camera,
    &page_camera_effect,

    &page_keyboard,
    &page_playback,
    &page_sketch,
    &page_sticker,
    &page_zoom,
    &page_screenshot,
    &page_glow,

    &page_taskbar,
};
extern bool g_is_menu_active;
int c_lvdsrx_chattering = 0;
int lvdsrx_setup_completed = 1;
extern uint32_t ramg_render_buffer_addr;
#define PIXEL_DUAL_2 3

int get_image_count()
{
    return images_count;
}

static void preload_pages()
{
    utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){0, 0, 255});

    for (int i = 0; i < NUM_PAGE; i++)
    {
        utils_paging *p = allpages[i];
        if (!p){
            continue;
        }

        if (p->load)
        {
            p->load();
        }
    }
    utils_displayEnd(PHOST);
}

static void preload_images()
{
#define ENABLE_BG 1
    for (int i = 0; i < get_image_count(); i++)
    {
        char msg[200];
        uint32_t image_load_status = 0;
        if (&demo_images[i] == IMG_LOGO_1920)
        {
            continue;
        }

#if ENABLE_BG
        utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){0, 0, 255});
        utils_drawImageCenter(PHOST, IMG_LOGO_1920, 0, 0, PHOST->Width, PHOST->Height);

        int percentage = 100 * i / get_image_count();
        snprintf(msg, 200, "Loading %d image (%d%%)", i + 1, percentage);
        EVE_CoCmd_text(PHOST, PHOST->Width / 2, PHOST->Height / 2 + 250, 30, OPT_CENTER, msg);
#endif

        // find image name
        int strpos = 0;
        int slen = min(99, strlen(demo_images[i].sd_path));
        for (int j = slen - 1; j >= 0; j--)
        {
            if (demo_images[i].sd_path[j] == '/' || demo_images[i].sd_path[j] == '\\')
            {
                strpos = slen - j;
                break;
            }
        }

        snprintf(msg, 200, "Loading image %s", &demo_images[i].sd_path[strpos]);
#if ENABLE_BG

        // draw bottom left screen
        EVE_CoCmd_text(PHOST, 50, PHOST->Height - 50, 30, 0, msg);
#endif
        image_load_status = utils_loadImageFromSdCard(PHOST, &demo_images[i]);
        if (image_load_status || !demo_images[i].is_loaded) // error with the SDcard reading
        {
#if !ENABLE_BG
            utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){0, 0, 255});
            cmd_fillwidth(PHOST, PHOST->Width * 8 / 10);
            EVE_CoCmd_text(PHOST, PHOST->Width / 2, PHOST->Height - 50, 30, OPT_FILL | OPT_CENTER, msg);
#endif
            printf("ERROR loading image!\r\n");
            EVE_CoCmd_text(PHOST, PHOST->Width / 2 - 300, PHOST->Height - 500, 32, 0, "ERROR loading image!");

            utils_displayEnd(PHOST);
            while (1)
            {
                EVE_sleep(1000);
            }
        }
#if ENABLE_BG
        utils_displayEnd(PHOST);
#endif
    }

    // preload sticker image
    page_sticker.init();
}

static void splash_screen()
{
    botom_center_y  = cam_geo.h + (demo_geo.h - cam_geo.h) / 2;

    utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
    vertex_format(PHOST, 1);
    Image *img = IMG_LOGO_1920;
    uint32_t image_load_status = utils_loadImageFromSdCard(PHOST, img);
    if (image_load_status || !img->is_loaded) // error with the SDcard reading
    {
        char msg[100] = {0};
        snprintf(msg, 100, "ERROR loading image %s!\r\n", img->sd_path);
        printf("%s", msg);
        EVE_CoCmd_text(PHOST, 10, 10, 32, 0, msg);
    }
    else
    {
        utils_drawImageCenter(PHOST, img, 0, 0, PHOST->Width, PHOST->Height);
    }
    utils_displayEnd(PHOST);

    while (image_load_status || !img->is_loaded)
    {
        EVE_sleep(1000);
    }

    preload_images();
}

static void init()
{
    page_sketch.isActive = 1;
    page_glow.isActive = 0;
    page_keyboard.isActive = 0;
    page_camera_effect.isActive = 0;
    page_playback.isActive = 0;
    page_zoom.isActive = 0;
    page_screenshot.isActive = 0;
    page_sticker.isActive = 0;

    page_camera.isActive = 1;
    page_taskbar.isActive = 1;

    // init EVE's DDR memory
    const uint32_t max_ramg = 2 * 1024 * 1024 * 1024; // maximum 2 GB of memory
    utils_ddrInit(PHOST->DDR_RamSize > 0 ? PHOST->DDR_RamSize : max_ramg);

#define ENABLE_SCREEN_ROTATE 0
#if ENABLE_SCREEN_ROTATE
    utils_lvdsConfig(PHOST, YCBCR, TESTCASE_SWAPPING);
    cmd_setrotate(PHOST, 1);
    utils_calibrateNew(PHOST);
#endif

    preload_pages();
    splash_screen();
}

static void deinit() {}
static void process_event()
{
    Gesture_Touch_t *ges = utils_gestureRenew(PHOST);
}

static void drawing()
{
    utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){255, 255, 255});
    vertex_format(PHOST, 1);
    if (g_is_menu_active)
    {
        utils_pageRun1(&page_camera);
        utils_pageRun1(&page_taskbar);
    }
    else
    {
        if (page_camera_effect.isActive || page_zoom.isActive || page_screenshot.isActive)
        {
            page_camera.isActive = 0;
        }
        else
        {
            page_camera.isActive = 1;
        }
        // loop through all pages
        utils_pageRun(allpages, NUM_PAGE);
    }
    utils_displayEnd(PHOST);
}

bool lvds_refresh()
{
    uint32_t lvdsrx_sts = rd32(PHOST, REG_LVDSRX_STAT);
    if ((lvdsrx_sts & 0xF0000000) != 0xF0000000)
    {
        c_lvdsrx_chattering++;

        if (lvdsrx_setup_completed == 1 && c_lvdsrx_chattering >= 3)
        {
            printf(" sync lost. cleared\n");
            wr32(PHOST, REG_RX_CAPTURE, 0);
            wr32(PHOST, REG_RX_ENABLE, 0);
            wr32(PHOST, REG_RX_ENABLE, 0);
            uint32_t lvdsrx_ctrl = rd32(PHOST, REG_LVDSRX_CTRL);
            lvdsrx_ctrl = lvdsrx_ctrl & 0xFFFFFEFE;
            wr32(PHOST, REG_LVDSRX_CTRL, lvdsrx_ctrl);
            c_lvdsrx_chattering = 0;
            lvdsrx_setup_completed = 0;
        }
    }
    else
    {
        c_lvdsrx_chattering = 0;
        if (lvdsrx_setup_completed == 0)
        {
            printf(" sync recovered. re-setup\n");

            wr32(PHOST, REG_SO_EN, 1);
            cmd_regwrite(PHOST, REG_RX_SETUP, 1);
            cmd_regwrite(PHOST, REG_RX_ENABLE, 1);
            wr32(PHOST, REG_RX_CAPTURE, 1);
            wr32(PHOST, REG_RX_ENABLE, 1);
            wr32(PHOST, REG_RX_ENABLE, 1);
            EVE_Cmd_waitFlush(PHOST);

            utils_scanoutSet();
            utils_scanout(&Swapchain_Render_Engine, ramg_render_buffer_addr, PIXEL_DUAL_2, RGB8);

            lvdsrx_setup_completed = 1;
        }
    }

    return false;
}

void EW2024_Photobooth()
{
    wr32(PHOST, REG_CTOUCH_EXTENDED, 0);
    init();
    while (1)
    {
        lvds_refresh();
        drawing();
        process_event();
    }
    deinit();
}