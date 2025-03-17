/**
 * @file main.c
 * @brief Main entry
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth_Utils.h"
#include "EW2024_Photobooth.h"

#define NEW_CALIBRATION 1
#define SET_CALIBRATION 1

void app_main(void)
{
    EVE_HalContext s_halContext;
    EVE_HalContext *s_pHalContext;

    s_pHalContext = &s_halContext;
    utils_hostInit(s_pHalContext);
    utils_gpuInit(PHOST);

    uint32_t result = 0;
    cmd_sdattach(PHOST, OPT_IS_SD | OPT_4BIT, &result);

    // read and store calibration setting
#if NEW_CALIBRATION == 1
    utils_lvdsConfig(PHOST, YCBCR, TESTCASE_SWAPPING);
    utils_calibrateNew(s_pHalContext);
#elif SET_CALIBRATION == 1
    utils_calibrateInit(PHOST, 0xfffefd52, 0x000002ab, 0x077be541, 0xfffffee4, 0xffff027a, 0x04a608f9);
#endif

    EVE_Util_clearScreen(PHOST);

    char *info[] =
        {"Photo Booth demo ",
         "Compatible with: BT820, ESP32S3, LCD 1920x1200",
         "",
         ""};
    utils_welcomeScreen(PHOST, info);

    while (1)
    {
        EW2024_Photobooth(PHOST);

        EVE_Util_clearScreen(PHOST);

        EVE_Hal_close(PHOST);
        EVE_Hal_release();

        /* Init HW Hal for next loop*/
        utils_gpuInit(PHOST);
    }
}
