/**
 * @file Gesture.h
 * @brief Header file for gesture functions.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

 #ifndef GESTURE_H_
 #define GESTURE_H_
 
 #include "EVE_CoCmd.h"
 
 #define MAX_FINGER 5
 
 typedef struct
 {
	 uint16_t touchX;
	 uint16_t touchY;
 } Gesture_Finger_t;
 
 typedef struct Gesture_Touch
 {
	 uint32_t tagTrackTouched;
	 uint32_t tagPressed;
	 uint32_t tagPressed1;
	 uint32_t tagReleased;
	 uint32_t tagVelocity;
	 uint32_t trackValLine;
	 uint32_t trackValCircle;
	 uint32_t longTapMilisecond;
	 uint32_t doubleTapMilisecond;
	 uint8_t isTouch;
	 int8_t isTouch3x; // touch 3x continue
	 uint8_t isSwipeX;
	 uint8_t isSwipeY;
	 uint8_t isSwipe;
	 uint8_t isDoubleTapTag;
	 uint8_t isDoubleTapXY;
	 uint8_t isLongTap;
	 uint16_t touchX;
	 uint16_t touchY;
	 int velocityX;
	 int velocityY;
	 int distance;
	 int distanceX;
	 int distanceY;
	 
	 int velocityX_total;
	 int velocityY_total;
 
	 int distance_scroll_X;
	 int distance_scroll_Y;
 
	 uint8_t velocityStopRequest;
 
	 Gesture_Finger_t finger[MAX_FINGER];
 } Gesture_Touch_t;
 
 Gesture_Touch_t* utils_gestureRenew(EVE_HalContext* phost);
 Gesture_Touch_t* utils_gestureGet();
 void stopVelocity();
 
 #endif /* GESTURE_H_ */
 