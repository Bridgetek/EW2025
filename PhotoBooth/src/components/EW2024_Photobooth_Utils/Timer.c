/**
 * @file Timer.c
 * @brief Timer utilities for delay and interval management
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "Timer.h"

void utils_timerStart(Timer *timer, uint32_t timeout, uint32_t interval)
{
    timer->startTime = millis();
    timer->timeout = timeout;
    timer->interval = interval;
    timer->count = 0;
}

bool utils_timerIsExpired(Timer *timer)
{
    // time already expired
    if (timer->count)
    {
        return true;
    }
    // time not started
    if (timer->startTime == 0)
    {
        return true;
    }

    uint32_t now = millis();
    uint32_t elapsed = now - timer->startTime;
    if (timer->timeout != 0 && elapsed >= timer->timeout)
    {
        timer->count++;
        return true;
    }
    return false;
}

bool utils_timerInterval(Timer *timer)
{
    uint32_t now = millis();
    uint32_t elapsed = now - timer->startTime;
    if (timer->interval != 0 && elapsed >= timer->interval)
    {
        timer->count++;
        timer->startTime = now;
        return true;
    }
    return false;
}

/**
 * Example function to show how to use utils_timerIsExpired, utils_timerStart and utils_timerInterval
 * @return void
 */
void Timer_example()
{
    Timer timer;
    // Start the timer with a 5-second timeout and an interval of 1-second
    utils_timerStart(&timer, 5000, 1000);
    while (1)
    {
        if (utils_timerIsExpired(&timer))
        {
            printf("Timer expired\n");
            utils_timerStart(&timer, 5000, 1000);
        }
        if (utils_timerInterval(&timer))
        {
            printf("Timer interval\n");
        }
        EVE_sleep(10);
    }
}
