/**
 * @file Image.h
 * @brief Image processing utilities
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "EW2024_Photobooth_Utils.h"

#define HW_CALC(rotate, w, h) (abs((sin(rotate / 180 * PI)) * w) + abs((cos(rotate / 180 * PI)) * h))
#define HH_CALC(rotate, w, h) (abs((cos(rotate / 180 * PI)) * w) + abs((sin(rotate / 180 * PI)) * h))

#define Load_PNG Load_Image
#define BMHL_AUTO 99
typedef struct
{

    /* Rotation of the image: 0-360 degree */
    uint16_t rotate;

    /* Horizontal width of the image,
          This is the width of the image after it has been rotated.
     * For example, if an image is rotated 90 degree, the width become the height of
          the image and the height become the width of the image.
     * The horizontal width of the image is the width of the image after it has been rotated.

       Calculated by abs(sin(rotate/180*PI)*w) + abs(cos(rotate/180*PI)*h)
     */
    uint16_t hw;

    /* Horizontal height of the image */
    uint16_t hh;

} Image_Rotate;

typedef struct
{

    /* Scale of the image */
    uint16_t scale;

    /* X-coordinate of the rotation center */
    uint16_t cx;

    /* Y-coordinate of the rotation center */
    uint16_t cy;

} Image_Scaling;

typedef enum
{
    IMG_FMT_PNG = 0,
    IMG_FMT_JPEG,
    IMG_FMT_RAW
} Image_Format;

typedef struct
{
    uint32_t tagval;
    uint32_t id;
    /* Pointer to the data */
    uint8_t *sd_path;

    /* X-coordinate of top-left corner */
    uint16_t x;

    /* Y-coordinate of top-left corner */
    uint16_t y;

    /* Width of the image */
    uint16_t w;

    /* Height of the image */
    uint16_t h;
    Image_Format fmt;
    uint16_t bitmap_format;
    int16_t bitmap_handler; // -1 = discard handler
    uint16_t opt;
    Image_Rotate rotate;
    bool is_loaded;
    uint32_t ptr; // RAM_G or flash address
    bool isOnFlash;

    uint32_t scale; // for scaling, 100 = no scale
} Image;

uint32_t utils_loadImageFromSdCard(EVE_HalContext *phost, Image *image);
uint32_t utils_drawImageXY(EVE_HalContext *phost, Image *image, uint32_t x, uint32_t y);
uint32_t utils_drawImageXyTag(EVE_HalContext *phost, Image *image, uint32_t x, uint32_t y, uint32_t tag);
uint32_t utils_drawImage(EVE_HalContext *phost, Image *image);
uint32_t utils_drawImageCenter(EVE_HalContext *phost, Image *image, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

#endif // IMAGE_H_