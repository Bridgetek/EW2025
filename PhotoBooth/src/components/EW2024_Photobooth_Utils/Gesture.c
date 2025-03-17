/**
 * @file Gesture.c
 * @brief Gesture recognition functions for the FT8xx EVE display processor
 *
 * This module provides functions to recognize gestures such as swipes, taps, and pinches from the touch data
 * provided by the FT8xx EVE display processor. The module uses the touch data to detect the type of gesture
 * and the corresponding state of the gesture (e.g. start, move, end). The module also provides functions to
 * get the information of the gesture, such as the position, direction, and scale of the gesture.
 *
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "EVE_Platform.h"

#include "EW2024_Photobooth_Utils.h"
#include "Gesture.h"

static Gesture_Touch_t sGesture;
static const int sMinMove = 15;

#define millis() (EVE_millis() / 1000) // Only for ESP32

// Constants
#define DOUBLE_TAP_THRESHOLD 500 // Maximum time in ms between taps
#define MAX_TAP_DISTANCE 50		 // Maximum distance in pixels between taps

/**
 * @brief Get horizontal velocity
 *
 * @return int horizontal velocity value
 */
static int Getvelocity_X()
{
	static uint16_t _delay = 0;
	static int lastTouchX = 0;
	static uint32_t prev_time = 0;
	uint32_t time = millis();
	static int distance = 0;
	static long dragprev = 0;
	static int velocityX = 0;
	static int count1Touch = 0;

	if (prev_time != 0)
	{
		_delay += (time - prev_time);
	}
	prev_time = time;

	if (_delay < 30)
	{
		return velocityX;
	}
	_delay = 0;

	if (sGesture.isTouch)
	{
		if (sGesture.isSwipe == 0)
		{
			if (lastTouchX == 0)
			{
				lastTouchX = sGesture.touchX;
			}
			else
			{
				distance = sGesture.touchX - lastTouchX;
			}
			if (abs(distance) > sMinMove)
			{ // this is a swipe
				sGesture.isSwipe = 1;
			}
		}
	}
	else
	{
		distance = 0;
		lastTouchX = 0;
		sGesture.isSwipe = 0;
	}

	if (sGesture.isSwipe == 1)
	{
		velocityX = (dragprev - sGesture.touchX) << 4;
	}
	else if (!sGesture.isTouch)
	{ // no touch
		int change = MAX(1, abs(velocityX) >> 5);
		if (velocityX < 0)
			velocityX += change;
		if (velocityX > 0)
			velocityX -= change;
	}
	else
	{
		count1Touch++;
		if (count1Touch == 5)
		{
			count1Touch = 0;
			velocityX = 0; // reset velocity when single touch
		}
	}

	dragprev = sGesture.touchX;
	return velocityX;
}

/**
 * @brief Get the Tag number of Released object
 *
 * @return uint8_t Tag number
 */
static uint32_t GetTagReleased()
{
	static uint32_t oldtag = 0;
	uint32_t newTag = 0;
	uint32_t tagRet = 0;

	newTag = sGesture.tagPressed;

	if (newTag == 0 && oldtag != 0)
	{
		tagRet = oldtag;
		oldtag = 0;
	}
	else if (newTag != 0)
	{
		oldtag = newTag;
	}

	return tagRet;
}

/**
 * @brief Return true if user long tap on screen
 *
 * @return true if user long tap on screen
 */
static bool isLongTap()
{
	static uint32_t longTapStartTime = 0;
	uint32_t time = millis();
	bool isLongTap = false;

	static uint32_t firstTapPointx = 0;
	static uint32_t firstTapPointy = 0;

	if (sGesture.isTouch)
	{
		uint32_t x = sGesture.touchX, y = sGesture.touchY;
		uint32_t distance = (firstTapPointx - x) * (firstTapPointx - x) +
							(firstTapPointy - y) * (firstTapPointy - y);
		firstTapPointx = x;
		firstTapPointy = y;
		if (longTapStartTime == 0)
		{
			longTapStartTime = time;
		}
		else if (time - longTapStartTime > sGesture.longTapMilisecond && distance <= MAX_TAP_DISTANCE * MAX_TAP_DISTANCE)
		{
			isLongTap = true;
		}
	}
	else
	{
		longTapStartTime = 0;
	}

	return isLongTap;
}

/**
 * @brief Return true if user double tap on screen
 *
 * @return true if user double tap on screen
 */

// Variables to track touch state

static bool isDoubleTapTag()
{
// Structures
#define DOUBLE_TAP_THRESHOLD 500 // Maximum time in ms between taps
	static bool firstTapDetected = false, tapAndReleaseDetected = false;
	static uint32_t firstTapTime = 0;
	uint32_t currentTime = millis();

	static uint32_t firstTapTag = 0;
	uint32_t tag = sGesture.tagPressed;

	if (sGesture.isTouch && tag != 0)
	{

		if (!firstTapDetected)
		{
			// First tap detected
			firstTapDetected = true;
			tapAndReleaseDetected = false;
			firstTapTime = currentTime;
			firstTapTag = tag;
		}
		else if (tapAndReleaseDetected)
		{
			// Check for double tap
			uint32_t timeSinceFirstTap = currentTime - firstTapTime;
			if (timeSinceFirstTap <= DOUBLE_TAP_THRESHOLD && firstTapTag == tag)
			{
				// Double tap detected
				tapAndReleaseDetected = false;
				firstTapDetected = false;
				return true;
			}
			else if (timeSinceFirstTap > DOUBLE_TAP_THRESHOLD)
			{
				// Too much time has passed, reset
				tapAndReleaseDetected = false;
				firstTapDetected = false;
			}
		}
	}
	else
	{
		if (firstTapDetected && (currentTime - firstTapTime) > DOUBLE_TAP_THRESHOLD)
		{
			// Reset if too much time has passed without a second tap
			firstTapDetected = false;
			tapAndReleaseDetected = false;
		}
		if (firstTapDetected)
		{
			tapAndReleaseDetected = true;
		}
	}

	return false;
}

static bool isDoubleTapXY()
{
// Structures
#define DOUBLE_TAP_THRESHOLD 500 // Maximum time in ms between taps
#define MAX_TAP_DISTANCE 50		 // Maximum distance in pixels between taps
	static bool firstTapDetected = false, tapAndReleaseDetected = false;
	static uint32_t firstTapTime = 0;
	uint32_t currentTime = millis();

	static uint32_t firstTapPointx = 0;
	static uint32_t firstTapPointy = 0;

	if (sGesture.isTouch)
	{
		uint32_t x = sGesture.touchX, y = sGesture.touchY;

		if (!firstTapDetected)
		{
			// First tap detected
			firstTapDetected = true;
			tapAndReleaseDetected = false;
			firstTapTime = currentTime;
			firstTapPointx = x;
			firstTapPointy = y;
		}
		else if (tapAndReleaseDetected)
		{
			// Check for double tap
			uint32_t timeSinceFirstTap = currentTime - firstTapTime;
			uint32_t distance = (firstTapPointx - x) * (firstTapPointx - x) +
								(firstTapPointy - y) * (firstTapPointy - y);
			if (timeSinceFirstTap <= DOUBLE_TAP_THRESHOLD && distance <= MAX_TAP_DISTANCE * MAX_TAP_DISTANCE)
			{
				// Double tap detected
				tapAndReleaseDetected = false;
				firstTapDetected = false;
				return true;
			}
			else if (timeSinceFirstTap > DOUBLE_TAP_THRESHOLD)
			{
				// Too much time has passed, reset
				tapAndReleaseDetected = false;
				firstTapDetected = false;
			}
		}
	}
	else
	{
		if (firstTapDetected && (currentTime - firstTapTime) > DOUBLE_TAP_THRESHOLD)
		{
			// Reset if too much time has passed without a second tap
			firstTapDetected = false;
			tapAndReleaseDetected = false;
		}
		if (firstTapDetected)
		{
			tapAndReleaseDetected = true;
		}
	}

	return false;
}

void measure_traveled_2finger()
{
	int x1, y1, x2, y2;
	int is_touching = 0; // Variable to track if the screen is being touched
	int distance = 0;	 // Variable to store the calculated distance

	// Simulate touch input
	{
		// Reset distance if no touch is detected
		if (x1 == -1)
		{
			is_touching = 0; // No touch detected
			distance = 0;	 // Reset distance
			printf("No touch detected, distance reset to 0.\n");
		}
		else
		{
			// If both fingers are touching, calculate distance
			is_touching = 1; // Fingers are touching
			distance = calculate_distance(x1, y1, x2, y2);
			printf("Current distance between fingers: %d pixels\n", distance);
		}
	}
}

void measure_traveled()
{
	static uint16_t x2 = 2, y2 = 0;
	static uint32_t isFirst = 1;

	if (!sGesture.isTouch)
	{
		x2 = 2;
		y2 = 0;
		isFirst = 1;
		return 0;
	}

	if (isFirst)
	{
		isFirst = 0;
		x2 = sGesture.touchX;
		y2 = sGesture.touchY;
	}
	sGesture.distanceX = sGesture.touchX - x2;
	sGesture.distanceY = sGesture.touchY - y2;

	int distanceABS = sqrt(sGesture.distanceX * sGesture.distanceX + sGesture.distanceY * sGesture.distanceY); // sqrt of (dx^2 + dy^2)
	sGesture.distance = distanceABS;

	if (sGesture.touchX < x2 || sGesture.touchY < y2)
	{
		sGesture.distance = -distanceABS;
	}
}
uint32_t GetTagPressed1()
{
	static uint32_t tagPressed = 0;
	if (tagPressed == sGesture.tagPressed)
	{
		return 0;
	}
	else
	{
		tagPressed = sGesture.tagPressed;
		return tagPressed;
	}
	return 0;
}

/**
 * @brief Refresh gesture data
 *
 */
Gesture_Touch_t *utils_gestureRenew(EVE_HalContext *phost)
{
#define EVE1_NO_TAG 0
#define EVE1_TAG_RESET 0xFF
#define EVE2_TAG_RESET 0xFFFFFFFF
#define EVE2_XY_RESET 0x80008000
#define EVE_TRACK_RESET 0xff
#define MAX_SCREEN_XY 2000

	uint32_t reg_touch_tag = EVE_Hal_rd32(phost, REG_TOUCH_TAG);
	uint32_t reg_track = EVE_Hal_rd32(phost, REG_TRACKER);
	uint32_t reg_touch_screen_xy = rd32(phost, REG_TOUCH_SCREEN_XY);

	uint32_t rawX = reg_touch_screen_xy >> 16;
	uint32_t rawY = reg_touch_screen_xy & 0x0000FFFF;

	sGesture.isTouch = (uint8_t)(reg_touch_screen_xy != EVE2_XY_RESET); // && sg_touchX < MAX_SCREEN_XY && sGesture.touchY < MAX_SCREEN_XY;
	sGesture.isTouch = sGesture.isTouch & (rawX >= 0 && rawX <= phost->Width);
	sGesture.isTouch = sGesture.isTouch & (rawY >= 0 && rawY <= phost->Height);

	if (sGesture.isTouch)
	{
		sGesture.isTouch3x++;
	}
	else
	{
		sGesture.isTouch3x--;
	}
	sGesture.isTouch3x = max(-3, min(3, sGesture.isTouch3x));

	sGesture.touchX = min(phost->Width, max(0, rawX));
	sGesture.touchY = min(phost->Height, max(0, rawY));

	sGesture.tagPressed = reg_touch_tag & EVE2_TAG_RESET;
	if (!sGesture.isTouch)
	{
		sGesture.tagPressed = 0;
	}

	sGesture.tagTrackTouched = reg_track;
	sGesture.trackValLine = reg_track >> 16;
	sGesture.trackValCircle = (reg_track >> 16) * 360 / 65535;
	sGesture.tagReleased = GetTagReleased();
	sGesture.tagPressed1 = GetTagPressed1();
	sGesture.velocityX = Getvelocity_X();

	sGesture.doubleTapMilisecond = 200;
	sGesture.isDoubleTapXY = isDoubleTapXY();
	sGesture.isDoubleTapTag = isDoubleTapTag();

	sGesture.longTapMilisecond = 800;
	sGesture.isLongTap = isLongTap();
	measure_traveled();

	uint32_t reg_touch_xy[] = {
		REG_CTOUCH_TOUCH0_XY,
		REG_CTOUCH_TOUCHA_XY,
		REG_CTOUCH_TOUCHB_XY,
		REG_CTOUCH_TOUCHC_XY,
		REG_CTOUCH_TOUCH4_XY,
	};

	for (int i = 0; i < MAX_FINGER; i++)
	{
		uint32_t touch_xy = rd32(phost, reg_touch_xy[i]);
		sGesture.finger[i].touchX = touch_xy >> 16;
		sGesture.finger[i].touchY = touch_xy & 0x0000FFFF;
	}

#define DEBUG_TAG_ON 0
#if DEBUG_TAG_ON
	if (sGesture.isTouch)
	{
		printf("touch x:%d y:%d, tag_reg: %d, tag pressed: %d, released: %d, "
			   "distance: %d, distancex: %d, distancey: %d, "
			   "isTouch3x: %d, "
			   "\n",
			   sGesture.touchX, sGesture.touchY, reg_touch_tag, sGesture.tagPressed, sGesture.tagReleased,
			   sGesture.distance, sGesture.distanceX, sGesture.distanceY,
			   sGesture.isTouch3x);
	}
	else if (sGesture.tagReleased > 0)
	{
		printf("tag released: %d\n", sGesture.tagReleased);
	}
#endif

	return &sGesture;
}

/**
 * @brief Get gesture data
 *
 * @return Gesture_Touch_t*
 */
Gesture_Touch_t *utils_gestureGet()
{
	return &sGesture;
}
