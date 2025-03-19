/**
 * @file Gesture.h
 * @brief Gesture recognition functions for touch screens
 *
 * This module provides functions to detect and handle gestures such as swipes,
 * taps, and pinches on touch screen devices. It processes touch input data
 * to identify gesture types and states, and offers utility functions to access
 * gesture information like position, direction, and velocity.
 *
 * @author Bridgetek
 *
 * @date 2025
 * @license MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 */

#ifndef GESTURE_H_
#define GESTURE_H_

#include "EVE_CoCmd.h"

typedef struct Gesture_Touch
{
	uint32_t tagTrackTouched;
	uint32_t tagPressed;
	uint32_t tagReleased;
	uint32_t trackValLine;
	uint32_t trackValCircle;
	uint8_t isTouch;
	uint8_t isSwipe;
	uint16_t touchX;
	uint16_t touchY;
	int velocityX;
} Gesture_Touch_t;

uint8_t Gesture_GetTag(EVE_HalContext *phost);
Gesture_Touch_t *Gesture_Renew(EVE_HalContext *phost);
Gesture_Touch_t *Gesture_Get();

#endif /* GESTURE_H_ */
