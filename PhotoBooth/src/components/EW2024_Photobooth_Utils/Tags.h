/**
 * @file Tags.h
 * @brief Header file for tag related functions
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef TAGS_H
#define TAGS_H

#define TAG_MIN 1
#define TAG_MAX 16777215
#define TAG_INC (TAG_MAX + 1)
#define TAG_SAME (TAG_INC + 1)
#define TAG_NONE 0

void utils_tagInit(/* no op */);
int utils_tagAlloc(uint32_t val);
extern uint32_t tag_counter;
uint32_t utils_tagNext();

#endif // TAGS_H