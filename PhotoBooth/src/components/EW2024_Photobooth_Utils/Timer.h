/**
 * @file Timer.h
 * @brief Timer management functions
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "EW2024_Photobooth_Utils.h"

#define millis() (EVE_millis() / 1000)

typedef struct Timer
{
    uint32_t startTime;
    uint32_t timeout;
    uint32_t interval;
    uint32_t count;
} Timer;

void utils_timerStart(Timer *timer, uint32_t timeout, uint32_t interval);
bool utils_timerIsExpired(Timer *timer);
bool utils_timerInterval(Timer *timer);

#endif
