/**
 * @file Tags.c
 * @brief Provides a unique identifier for each UI element.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "EW2024_Photobooth_Utils.h"
#include "tags.h"

// Static variable to maintain the counter state
uint32_t tag_counter = 500; // 0-255 for keyboard with cmd_keys

// Function to return the next integer tag
int utils_tagAlloc(uint32_t val)
{
    if (tag_counter >= TAG_MAX)
    {
        APP_INF("ERROR: TAG_MAX reached");
        return 0;
    }

    if (val == TAG_SAME)
    {
        return tag_counter;
    }
    else if (val == TAG_INC)
    {
        uint32_t tagret = utils_tagNext();
        return tagret;
    }
    return val;
}

uint32_t utils_tagNext()
{
    tag_counter++;
    return tag_counter;
}