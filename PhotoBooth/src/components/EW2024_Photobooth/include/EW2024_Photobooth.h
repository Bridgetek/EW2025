/**
 * @file EW2024_Photobooth.h
 * @brief This is a demo for the EVE BT820 module on the
 * Bridgetek's EW2024 development board. It uses the EVE coprocessor to
 * accelerate graphics intensive tasks and implements a simple photobooth
 * application with features like image capture, video processing, and
 * video overlay.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef DEMO_PHOTOBOOTH_H_
#define DEMO_PHOTOBOOTH_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "EVE_Platform.h"
#include "EW2024_Photobooth_Utils.h"

    void demo_photobooth();

#define ASSET_DIR "assets_sdcard_eve/photobooth2024/"
#define EMOJI_DIR ASSET_DIR "emoji/"
#define IMG_DIR ASSET_DIR "img/"

#define IMG_LOGO (&demo_images[0])
#define IMG_TASKBAR (&demo_images[1])
#define IMG_ICON_MENU (&demo_images[2])
#define IMG_ICON_MENU_ON (&demo_images[3])
#define IMG_ICON_BOX_ON (&demo_images[4])
#define IMG_ICON_BOX (&demo_images[5])
#define IMG_ICON_EFFECT (&demo_images[6])
#define IMG_ICON_CROP (&demo_images[7])
#define IMG_ICON_BLUR (&demo_images[8])
#define IMG_ICON_GLOW (&demo_images[9])
#define IMG_ICON_KEYBOARD (&demo_images[10])
#define IMG_ICON_PLAYBACK (&demo_images[11])
#define IMG_ICON_SKETCH (&demo_images[12])
#define IMG_ICON_STICKER (&demo_images[13])
#define IMG_ICON_ZOOM (&demo_images[14])
#define IMG_BTN_BACK (&demo_images[15])
#define IMG_LOGO_1920 (&demo_images[16])
#define IMG_ICON_COLOR_PICKER (&demo_images[17])
#define IMG_ICON_LINE_WIDTH1 (&demo_images[18])
#define IMG_ICON_LINE_WIDTH2 (&demo_images[19])
#define IMG_ICON_LINE_WIDTH3 (&demo_images[20])
#define IMG_ICON_LINE_WIDTH4 (&demo_images[21])
#define IMG_ICON_PAINTFRAME (&demo_images[22])
#define IMG_ICON_CLEAN (&demo_images[23])
#define IMG_ICON_CLEANALL (&demo_images[24])
#define IMG_ICON_CLOSE (&demo_images[25])
#define IMG_ICON_EXPAND (&demo_images[26])
#define IMG_ICON_MOVE (&demo_images[27])
#define IMG_ICON_MORE (&demo_images[29])
#define IMG_ICON_ZOOMIN (&demo_images[30])
#define IMG_ICON_ZOOMOUT (&demo_images[31])
#define IMG_ICON_UPARROW (&demo_images[32])

#define images_count sizeof(demo_images) / sizeof(demo_images[0])

// paging
#define NUM_PAGE sizeof(allpages) / sizeof(allpages[0])
    extern utils_paging *allpages[];

    // individual pages
    extern utils_paging page_camera_effect;
    extern utils_paging page_keyboard;
    extern utils_paging page_playback;
    extern utils_paging page_sketch;
    extern utils_paging page_sticker;
    extern utils_paging page_zoom;
    extern utils_paging page_screenshot;
    extern utils_paging page_glow;

    // page activated usualy
    extern utils_paging page_camera;
    extern utils_paging page_taskbar;

    // globals
    extern Image demo_images[];
    extern box demo_geo;
    extern box cam_geo;
    extern uint32_t botom_center_y ;

    int get_image_count();

#ifdef __cplusplus
}
#endif

#endif /* DEMO_PHOTOBOOTH_H_ */
